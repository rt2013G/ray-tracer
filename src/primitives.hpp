#ifndef RAY_HPP
#define RAY_HPP

#include <algorithm>
#include <vector>

#include "canvas.hpp"
#include "matrix.hpp"
#include "vector.hpp"

struct ray {
    vec origin;
    vec direction;
    vec position(float t);
    ray transform(matrix4x4 T);
};

vec ray::position(float t) {
    return this->origin + this->direction * t;
}

ray ray::transform(matrix4x4 T) {
    ray r{
        T * this->origin,
        T * this->direction};
    return r;
};

struct point_light {
    vec position;
    color intensity;
};

struct material {
    color surface;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    material(color c, float a, float d, float sp, float sh);
    material();
};

material::material(color c, float a, float d, float sp, float sh) {
    this->surface = c;
    this->ambient = a;
    this->diffuse = d;
    this->specular = sp;
    this->shininess = sh;
}

material::material() {
    this->surface = WHITE;
    this->ambient = 0.1;
    this->diffuse = 0.9;
    this->specular = 0.9;
    this->shininess = 128;
}

struct object {
    int id;
    matrix4x4 transform;
    material mat;
    object();
    object(matrix4x4 transform);
    object(matrix4x4 transform, material mat);
};

static uint CURRENT_ID = 0;
object::object() {
    this->id = CURRENT_ID;
    CURRENT_ID++;
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

struct intersection {
    float t;
    object obj;
};

std::vector<intersection> intersect_sphere(object sphere, ray r) {
    r = r.transform(sphere.transform.inverse());
    vec sphere_to_r = r.origin - vect::point3(0, 0, 0);
    float a = r.direction.dot(r.direction);
    float b = 2 * r.direction.dot(sphere_to_r);
    float c = sphere_to_r.dot(sphere_to_r) - 1;

    float discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return std::vector<intersection>();
    }
    float t1 = (-b - sqrt(discriminant)) / (2 * a);
    float t2 = (-b + sqrt(discriminant)) / (2 * a);
    return std::vector<intersection>{intersection{t1, sphere}, intersection{t2, sphere}};
}

vec normal_at(object obj, vec p) {
    matrix4x4 invers_trans = obj.transform.inverse();
    vec obj_p = invers_trans * p;
    vec obj_normal = obj_p - vect::origin;
    vec world_normal = invers_trans.transpose() * obj_normal;
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

color phong_lighting(material mat, vec p, point_light light, vec eye, vec normal) {
    color effective_color = mat.surface * light.intensity;
    color ambient = effective_color * mat.ambient;
    color diffuse = BLACK;
    color specular = BLACK;

    vec light_dir = (light.position - p).normalize();
    float light_dot_normal = light_dir.dot(normal);
    if (light_dot_normal >= 0) {
        diffuse = effective_color * mat.diffuse * light_dot_normal;
        vec reflection = vect::reflect(-light_dir, normal);
        float reflect_dot_eye = reflection.dot(eye);
        if (reflect_dot_eye > 0) {
            specular = light.intensity * mat.specular * pow(reflect_dot_eye, mat.shininess);
        }
    }

    return ambient + diffuse + specular;
}

struct computation {
    float t;
    object obj;
    vec point;
    vec eye_vec;
    vec normal_vec;
    bool inside;
    computation(intersection i, ray r);
};

computation::computation(intersection i, ray r) {
    this->t = i.t;
    this->obj = i.obj;
    this->point = r.position(i.t);
    this->eye_vec = -r.direction;
    this->eye_vec.normalize();
    this->normal_vec = normal_at(i.obj, this->point);
    this->normal_vec.normalize();
    if (this->normal_vec.dot(this->eye_vec) < 0) {
        this->inside = true;
        this->normal_vec = -this->normal_vec;
    } else {
        this->inside = false;
    }
}

struct world {
    std::vector<point_light> plights;
    std::vector<object> objects;
    world();
    world(std::vector<point_light> plights, std::vector<object> objects);
    std::vector<intersection> intersect(ray r);
    color shade_hit(computation comp);
    color color_at(ray r);
};

world::world() {
    this->plights.push_back(point_light{vect::point3(-10, 10, -10), WHITE});
    object s1{};
    s1.mat.surface = color(0.8, 1, 0.6);
    s1.mat.diffuse = 0.7;
    s1.mat.specular = 0.2;
    object s2{};
    s2.transform = mat::scaling(0.5, 0.5, 0.5);
    this->objects.push_back(s1);
    this->objects.push_back(s2);
}

world::world(std::vector<point_light> plights, std::vector<object> objects) {
    this->plights = plights;
    this->objects = objects;
}

bool interesection_sorter(intersection &i1, intersection &i2) {
    return i1.t < i2.t;
}

std::vector<intersection> world::intersect(ray r) {
    std::vector<intersection> inters{};
    for (object &obj : this->objects) {
        std::vector<intersection> result = intersect_sphere(obj, r);
        for (intersection &i : result) {
            inters.push_back(i);
        }
    }
    std::sort(inters.begin(), inters.end(), interesection_sorter);
    return inters;
}

color world::shade_hit(computation comp) {
    color c = BLACK;
    for (point_light &pl : this->plights) {
        c = c + phong_lighting(comp.obj.mat, comp.point, pl, comp.eye_vec, comp.normal_vec);
    }
    return c;
}

color world::color_at(ray r) {
    std::vector<intersection> inters = this->intersect(r);
    intersection i = hit(inters);
    if (i.t < 0) {
        return BLACK;
    }
    computation comp{i, r};
    return this->shade_hit(comp);
}

struct camera {
    float hsize;
    float vsize;
    float fov;
    matrix4x4 transform;
    float half_width;
    float half_height;
    float pixel_size;
    void compute_pixel_size();
    camera();
    camera(int hsize, int vsize, float fov);
    camera(int hsize, int vsize, float fov, matrix4x4 transform);
    ray ray_for_pixel(int x, int y);
    canvas render(world w);
};

void camera::compute_pixel_size() {
    float half_view = tan(this->fov / 2);
    float aspect_ratio = this->hsize / this->vsize;
    if (aspect_ratio >= 1) {
        this->half_width = half_view;
        this->half_height = half_view / aspect_ratio;
    } else {
        this->half_width = half_view * aspect_ratio;
        this->half_height = half_view;
    }
    this->pixel_size = (this->half_width * 2) / this->hsize;
}

camera::camera() {
    this->hsize = 160;
    this->vsize = 120;
    this->fov = M_PI / 2;
    this->transform = mat::identity;
    this->compute_pixel_size();
}

camera::camera(int hsize, int vsize, float fov) {
    this->hsize = hsize;
    this->vsize = vsize;
    this->fov = fov;
    this->transform = mat::identity;
    this->compute_pixel_size();
}

camera::camera(int hsize, int vsize, float fov, matrix4x4 transform) {
    this->hsize = hsize;
    this->vsize = vsize;
    this->fov = fov;
    this->transform = transform;
    this->compute_pixel_size();
}

ray camera::ray_for_pixel(int px, int py) {
    float x_offset = (px + 0.5) * this->pixel_size;
    float y_offset = (py + 0.5) * this->pixel_size;
    float world_x = this->half_width - x_offset;
    float world_y = this->half_height - y_offset;
    matrix4x4 inv_transf = this->transform.inverse();
    vec v = vect::point3(world_x, world_y, -1);
    vec pixel = inv_transf * v;
    vec o = vect::point3(0, 0, 0);
    vec origin = inv_transf * o;
    vec direction = (pixel - origin).normalize();

    return ray{origin, direction};
}

canvas camera::render(world w) {
    canvas c{int(this->hsize), int(this->vsize)};
    for (int y = 0; y < this->vsize; y++) {
        for (int x = 0; x < this->hsize; x++) {
            ray r{this->ray_for_pixel(x, y)};
            color col = w.color_at(r);
            c.write(x, y, col);
        }
    }

    return c;
}

#endif