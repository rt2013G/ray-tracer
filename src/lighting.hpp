#ifndef LIGHTING_HPP
#define LIGHTING_HPP

#include <algorithm>
#include <vector>

#include "canvas.hpp"
#include "matrix.hpp"
#include "object.hpp"
#include "vector.hpp"

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
    r = r.transform(sphere.transform.inverse());
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

vector normal_at(object obj, vector p) {
    matrix4x4 invers_trans = obj.transform.inverse();
    vector obj_p = invers_trans * p;
    vector obj_normal = obj_p - vec::origin;
    vector world_normal = invers_trans.transpose() * obj_normal;
    world_normal.w = 0;
    return world_normal.normalize();
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