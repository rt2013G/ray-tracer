#ifndef RAY_HPP
#define RAY_HPP

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

struct object {
};

struct intersection {
    float t;
    object obj;
};

intersection hit(std::vector<intersection> &inters) {
    float t_min = INFINITY;
    intersection hit = inters[0];
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

struct sphere : object {
    int id;
    matrix4x4 transform;
    sphere(int id);
    std::vector<intersection> interesect(ray r);
};

sphere::sphere(int id) {
    this->id = id;
    this->transform = mat::identity;
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
    return std::vector<intersection>{intersection{t1, *this}, intersection{t2, *this}};
}

#endif