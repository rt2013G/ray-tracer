#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"

struct matrix4x4 {
    float val[16];
    bool operator==(matrix4x4 &mat);
    matrix4x4 operator*(matrix4x4 &mat);
    vec operator*(vec &v);
};

bool matrix4x4::operator==(matrix4x4 &mat) {
    for (int i = 0; i < 16; i++) {
        if (this->val[i] != mat.val[i]) {
            return false;
        }
    }
    return true;
}

vec matrix4x4::operator*(vec &v) {
    vec out{0, 0, 0, 0};
    out.x += this->val[0] * v.x;
    out.x += this->val[1] * v.y;
    out.x += this->val[2] * v.z;
    out.x += this->val[3] * v.w;
    out.y += this->val[4] * v.x;
    out.y += this->val[5] * v.y;
    out.y += this->val[6] * v.z;
    out.y += this->val[7] * v.w;
    out.z += this->val[8] * v.x;
    out.z += this->val[9] * v.y;
    out.z += this->val[10] * v.z;
    out.z += this->val[11] * v.w;
    out.w += this->val[12] * v.x;
    out.w += this->val[13] * v.y;
    out.w += this->val[14] * v.z;
    out.w += this->val[15] * v.w;
    return out;
}

matrix4x4 matrix4x4::operator*(matrix4x4 &mat) {
    matrix4x4 c{};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                c.val[i * 4 + j] += this->val[i * 4 + k] * mat.val[k * 4 + j];
            }
        }
    }
    return c;
}

#endif