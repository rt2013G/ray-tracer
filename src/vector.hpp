#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "math.h"

#define EPSILON 0.01f
#define SHADOW_OFFSET 0.01f

bool eq(const float &f1, const float &f2) {
    if (abs(f1 - f2) < EPSILON) {
        return true;
    } else {
        return false;
    }
}

struct vector {
    float x;
    float y;
    float z;
    float w;
    bool operator==(const vector &a);
    bool operator!=(const vector &a);
    vector operator+(const vector &a);
    vector operator-(const vector &a);
    vector operator-();
    vector operator*(const float &c);
    float mag();
    vector normalize();
    void print();
};

bool vector::operator==(const vector &a) {
    if (eq(this->x, a.x) && eq(this->y, a.y) && eq(this->z, a.z) && eq(this->w, a.w)) {
        return true;
    } else {
        return false;
    }
};

bool vector::operator!=(const vector &a) {
    if (*this == a) {
        return false;
    } else {
        return true;
    }
}

vector vector::operator+(const vector &a) {
    return vector{
        this->x + a.x,
        this->y + a.y,
        this->z + a.z,
        this->w + a.w};
};

vector vector::operator-(const vector &a) {
    return vector{
        this->x - a.x,
        this->y - a.y,
        this->z - a.z,
        this->w - a.w};
};

vector vector::operator-() {
    return vector{
        -this->x,
        -this->y,
        -this->z,
        -this->w};
};

vector vector::operator*(const float &c) {
    return vector{
        this->x * c,
        this->y * c,
        this->z * c,
        this->w,
    };
};

float vector::mag() {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
};

vector vector::normalize() {
    float mag = this->mag();
    return vector{this->x /= mag,
                  this->y /= mag,
                  this->z /= mag, this->w};
};

void vector::print() {
    std::cout << this->x << " " << this->y << " " << this->z << " " << this->w << std::endl;
}

namespace vec {
vector point3(float x, float y, float z) {
    return vector{x, y, z, 1};
}

vector vector3(float x, float y, float z) {
    return vector{x, y, z, 0};
}

const vector origin = point3(0, 0, 0);

float dot(const vector &a, const vector &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vector cross(const vector &a, const vector &b) {
    return vector{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        0,
    };
};

vector reflect(vector in, vector normal) {
    return in - normal * 2 * dot(in, normal);
}

} // namespace vec

#endif