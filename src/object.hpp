#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "algorithm"

#include "color.hpp"
#include "matrix.hpp"

struct material {
    color surface;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    float reflective;
    float transparency;
    material(color surface, float ambient, float diffuse, float specular, float shininess, float reflective);
    material();
};

material::material(color surface, float ambient, float diffuse, float specular, float shininess, float reflective) {
    this->surface = surface;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
    this->reflective = reflective;
}

material::material() {
    this->surface = WHITE;
    this->ambient = 0.1;
    this->diffuse = 0.9;
    this->specular = 0.9;
    this->shininess = 128;
    this->reflective = 0.0;
}

struct triangle_data {
    vector p1;
    vector p2;
    vector p3;
    vector e1;
    vector e2;
    vector normal;
    triangle_data(vector p1, vector p2, vector p3);
};

triangle_data::triangle_data(vector p1, vector p2, vector p3) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->e1 = p2 - p1;
    this->e2 = p3 - p1;
    this->normal = vec::cross(e2, e1).normalize();
}

enum obj_type {
    SPHERE = 0,
    PLANE = 1,
    TRIANGLE = 2
};

struct object {
    int id;
    obj_type type = SPHERE;
    matrix4x4 transform;
    material mat;
    object();
    object(obj_type type);
    object(matrix4x4 transform);
    object(matrix4x4 transform, material mat);
    triangle_data tdata = triangle_data{vec::origin, vec::origin, vec::origin};
};

static uint CURRENT_ID = 0;
object::object() {
    this->id = CURRENT_ID;
    CURRENT_ID++;
    this->transform = mat::identity;
    this->mat = material();
}

object::object(obj_type type) {
    this->id = CURRENT_ID;
    CURRENT_ID++;
    this->type = type;
    this->transform = mat::identity;
    this->mat = material();
}

object::object(matrix4x4 transform) {
    this->id = CURRENT_ID;
    CURRENT_ID++;
    this->transform = transform;
    this->mat = material();
}

object::object(matrix4x4 transform, material mat) {
    this->id = CURRENT_ID;
    CURRENT_ID++;
    this->transform = transform;
    this->mat = mat;
}

object triangle(vector p1, vector p2, vector p3) {
    object triangle{TRIANGLE};
    triangle.tdata = triangle_data(p1, p2, p3);
    return triangle;
}

vector normal_at(object obj, vector p) {
    matrix4x4 invers_trans = obj.transform.inverse();
    vector obj_p = invers_trans * p;
    vector obj_normal = vec::vector3(0, 0, 0);
    switch (obj.type) {
    case SPHERE: {
        obj_normal = obj_p - vec::origin;
    } break;
    case PLANE: {
        obj_normal = vec::vector3(0, 1, 0);
    } break;
    case TRIANGLE: {
        obj_normal = obj.tdata.normal;
    } break;
    default: {
        std::cout << "ERROR. unidentified object type" << std::endl;
    };
    }
    vector world_normal = invers_trans.transpose() * obj_normal;
    world_normal.w = 0;
    return world_normal.normalize();
}

namespace ray {

struct ray {
    vector origin;
    vector direction;
    vector find_position(float t);
    ray transform(matrix4x4 T);
};

vector ray::find_position(float t) {
    return this->origin + this->direction * t;
}

ray ray::transform(matrix4x4 T) {
    ray r{
        T * this->origin,
        T * this->direction};
    return r;
};

struct intersection {
    float t;
    object obj;
};

std::vector<intersection> intersect_sphere(object sphere, ray r) {
    vector sphere_to_r = r.origin - vec::point3(0, 0, 0);
    float a = vec::dot(r.direction, r.direction);
    float b = 2 * vec::dot(r.direction, sphere_to_r);
    float c = vec::dot(sphere_to_r, sphere_to_r) - 1;

    float discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return std::vector<intersection>();
    }
    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);
    return std::vector<intersection>{intersection{t1, sphere}, intersection{t2, sphere}};
}

std::vector<intersection> intersect_plane(object plane, ray r) {
    if (abs(r.direction.y) < EPSILON) {
        return std::vector<intersection>{};
    }

    float t = -r.origin.y / r.direction.y;
    return std::vector<intersection>{intersection{t, plane}};
}

std::vector<intersection> intersect_triangle(object t, ray r) {
    vector dir_cross_e2 = vec::cross(r.direction, t.tdata.e2);
    float det = vec::dot(t.tdata.e1, dir_cross_e2);
    if (abs(det) < EPSILON) {
        return std::vector<intersection>{};
    }

    float f = 1.0 / det;
    vector p1_to_origin = r.origin - t.tdata.p1;
    float u = f * vec::dot(p1_to_origin, dir_cross_e2);
    if (u < 0 || u > 1) {
        return std::vector<intersection>{};
    }

    vector origin_cross_e1 = vec::cross(p1_to_origin, t.tdata.e1);
    float v = f * vec::dot(r.direction, origin_cross_e1);
    if (v < 0 || (u + v) > 1) {
        return std::vector<intersection>{};
    }

    float td = f * vec::dot(t.tdata.e2, origin_cross_e1);
    return std::vector<intersection>{intersection{td, t}};
}

std::vector<intersection> intersect_object(object obj, ray r) {
    r = r.transform(obj.transform.inverse());
    switch (obj.type) {
    case SPHERE: {
        return intersect_sphere(obj, r);
    } break;
    case PLANE: {
        return intersect_plane(obj, r);
    } break;
    default: {
        std::cout << "ERROR. unidentified object type" << std::endl;
        return std::vector<intersection>{};
    }
    }
}

intersection hit(std::vector<intersection> &inters) {
    float t_min = INFINITY;
    intersection hit{-INFINITY};
    if (inters.size() == 0) {
        return hit;
    }
    for (intersection &i : inters) {
        if (i.t < 0) {
            continue;
        }
        if (i.t < t_min) {
            t_min = i.t;
            hit = i;
        }
    }
    return hit;
}

struct computation {
    float t;
    object obj;
    vector point;
    vector over_point;
    vector eye_vec;
    vector normal_vec;
    vector reflect_vec;
    bool inside;
    computation(intersection i, ray r);
};

computation::computation(intersection i, ray r) {
    this->t = i.t;
    this->obj = i.obj;
    this->point = r.find_position(i.t);
    this->eye_vec = (-r.direction).normalize();
    this->normal_vec = normal_at(i.obj, this->point).normalize();
    if (vec::dot(this->normal_vec, this->eye_vec) < 0) {
        this->inside = true;
        this->normal_vec = -this->normal_vec;
    } else {
        this->inside = false;
    }
    this->over_point = this->point + this->normal_vec * SHADOW_OFFSET;
    this->reflect_vec = vec::reflect(r.direction, this->normal_vec);
}
} // namespace ray

struct point_light {
    vector position;
    color intensity;
};

color phong_lighting(material mat, vector p, point_light light, vector eye, vector normal, bool in_shadow = false) {
    color effective_color = mat.surface * light.intensity;
    color ambient = effective_color * mat.ambient;
    if (in_shadow) {
        return ambient;
    }
    color diffuse = BLACK;
    color specular = BLACK;
    vector light_dir = (light.position - p).normalize();
    float light_dot_normal = vec::dot(light_dir, normal);
    if (light_dot_normal >= 0) {
        diffuse = effective_color * mat.diffuse * light_dot_normal;
        vector reflection = vec::reflect(-light_dir, normal);
        float reflect_dot_eye = vec::dot(reflection, eye);
        if (reflect_dot_eye > 0) {
            specular = light.intensity * mat.specular * pow(reflect_dot_eye, mat.shininess);
        }
    }

    return ambient + diffuse + specular;
}

#endif