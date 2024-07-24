#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "math.h"

const float EPSILON = 0.0001f;

bool eq(const float &f1, const float &f2) {
    if (abs(f1 - f2) < EPSILON) {
        return true;
    } else {
        return false;
    }
}

struct vec {
    float x;
    float y;
    float z;
    float w;
    bool operator==(const vec &a);
    vec operator+(const vec &a);
    vec operator-(const vec &a);
    vec operator-();
    vec operator*(const float &c);
    float mag();
    vec normalize();
    float dot(const vec &a);
    vec cross(const vec &a);
};

bool vec::operator==(const vec &a) {
    if (eq(this->x, a.x) && eq(this->y, a.y) && eq(this->z, a.z)) {
        return true;
    } else {
        return false;
    }
}

vec vec::operator+(const vec &a) {
    return vec{
        this->x + a.x,
        this->y + a.y,
        this->z + a.z,
        this->w + a.w};
};

vec vec::operator-(const vec &a) {
    return vec{
        this->x - a.x,
        this->y - a.y,
        this->z - a.z,
        this->w - a.w};
};

vec vec::operator-() {
    return vec{
        -this->x,
        -this->y,
        -this->z,
        -this->w};
};

vec vec::operator*(const float &c) {
    return vec{
        this->x *= c,
        this->y *= c,
        this->z *= c,
        this->w,
    };
};

float vec::mag() {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
};

vec vec::normalize() {
    float mag = this->mag();
    this->x /= mag;
    this->y /= mag;
    this->z /= mag;
    return *this;
};

float vec::dot(const vec &a) {
    return this->x * a.x + this->y * a.y + this->z * a.z + this->w * a.w;
}

vec vec::cross(const vec &a) {
    return vec{
        this->y * a.z - this->z * a.y,
        this->z * a.x - this->x * a.z,
        this->x * a.y - this->y * a.x,
        0,
    };
};

#endif