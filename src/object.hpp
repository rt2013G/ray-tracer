#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "color.hpp"
#include "matrix.hpp"

struct material {
    color surface;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
    material(color surface, float ambient, float diffuse, float specular, float shininess);
    material();
};

material::material(color surface, float ambient, float diffuse, float specular, float shininess) {
    this->surface = surface;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
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

#endif