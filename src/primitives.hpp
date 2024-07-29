#ifndef RAY_HPP
#define RAY_HPP

#include <algorithm>
#include <vector>

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

struct intersection {
    float t;
    int id;
};

struct object {
    int id;
    matrix4x4 transform;
    material mat;
    std::vector<intersection> interesect(ray r);
    vec normal_at(vec p);
};

intersection hit(std::vector<intersection> &inters) {
    float t_min = INFINITY;
    intersection hit{-INFINITY, {}};
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

struct sphere : object {
    int id;
    matrix4x4 transform;
    material mat;
    sphere(int id);
    std::vector<intersection> interesect(ray r);
    vec normal_at(vec p);
};

sphere::sphere(int id) {
    this->id = id;
    this->transform = mat::identity;
    this->mat = material();
}

std::vector<intersection> sphere::interesect(ray r) {
    r = r.transform(this->transform.inverse());
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
    return std::vector<intersection>{intersection{t1, this->id}, intersection{t2, this->id}};
}

vec sphere::normal_at(vec p) {
    vec obj_p = this->transform.inverse() * p;
    vec obj_normal = obj_p - vect::origin;
    vec world_normal = this->transform.inverse().transpose() * obj_normal;
    world_normal.w = 0;
    return world_normal.normalize();
}

struct world {
    std::vector<point_light> plights;
    std::vector<object> objects;
    world();
    std::vector<intersection> intersect(ray r);
};

bool interesection_sorter(intersection &i1, intersection &i2) {
    return i1.t < i2.t;
}

std::vector<intersection> world::intersect(ray r) {
    std::vector<intersection> inters{};
    for (object &obj : this->objects) {
        std::vector<intersection> result = obj.interesect(r);
        for (intersection &i : result) {
            inters.push_back(i);
        }
    }
    std::sort(inters.begin(), inters.end(), interesection_sorter);
    return inters;
}

world::world() {
    this->plights.push_back(point_light{vect::point3(-10, 10, -10), WHITE});
    sphere s{0};
    s.transform = mat::scaling(0.5, 0.5, 0.5);
    s.mat.surface = color(0.8, 1, 0.6);
    s.mat.diffuse = 0.7;
    s.mat.specular = 0.2;
    sphere s1{1};
    this->objects.push_back(s);
    this->objects.push_back(s1);
}

#endif