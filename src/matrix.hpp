#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"

struct matrix2x2 {
    float val[4];
    float det();
    bool operator==(matrix2x2 &mat);
};

float matrix2x2::det() {
    return this->val[0] * this->val[3] - this->val[1] * this->val[2];
}

bool matrix2x2::operator==(matrix2x2 &mat) {
    for (int i = 0; i < 4; i++) {
        if (!eq(this->val[i], mat.val[i])) {
            return false;
        }
    }
    return true;
}

struct matrix3x3 {
    float val[9];
    bool operator==(matrix3x3 &mat);
    matrix2x2 submatrix(int row, int col);
    float minor(int i, int j);
    float cofactor(int i, int j);
    float det();
};

bool matrix3x3::operator==(matrix3x3 &mat) {
    for (int i = 0; i < 9; i++) {
        if (!eq(this->val[i], mat.val[i])) {
            return false;
        }
    }
    return true;
}

matrix2x2 matrix3x3::submatrix(int row, int col) {
    matrix2x2 A;
    int a = 0, b = 0;
    for (int i = 0; i < 3; i++) {
        if (i == row) {
            continue;
        }
        for (int j = 0; j < 3; j++) {
            if (j == col) {
                continue;
            }
            A.val[a * 2 + b] = this->val[i * 3 + j];
            b++;
        }
        a++;
        b = 0;
    }
    return A;
}

float matrix3x3::minor(int i, int j) {
    matrix2x2 submat = this->submatrix(i, j);
    return submat.det();
}

float matrix3x3::cofactor(int i, int j) {
    float minor = this->minor(i, j);
    if ((i + j) % 2 == 0) {
        return minor;
    } else {
        return -minor;
    }
}

float matrix3x3::det() {
    float determinant = 0;
    for (int j = 0; j < 3; j++) {
        determinant += this->val[j] * this->cofactor(0, j);
    }
    return determinant;
}

struct matrix4x4 {
    float val[16];
    bool operator==(const matrix4x4 &mat);
    matrix4x4 operator*(matrix4x4 &mat);
    vec operator*(vec &v);
    matrix4x4 transpose();
    matrix3x3 submatrix(int row, int col);
    float minor(int i, int j);
    float cofactor(int i, int j);
    float det();
    matrix4x4 inverse();
    void print();
};

bool matrix4x4::operator==(const matrix4x4 &mat) {
    for (int i = 0; i < 16; i++) {
        if (!eq(this->val[i], mat.val[i])) {
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

matrix4x4 matrix4x4::transpose() {
    matrix4x4 c;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c.val[j * 4 + i] = this->val[i * 4 + j];
        }
    }
    return c;
}

matrix3x3 matrix4x4::submatrix(int row, int col) {
    matrix3x3 A;
    int a = 0, b = 0;
    for (int i = 0; i < 4; i++) {
        if (i == row) {
            continue;
        }
        for (int j = 0; j < 4; j++) {
            if (j == col) {
                continue;
            }
            A.val[a * 3 + b] = this->val[i * 4 + j];
            b++;
        }
        a++;
        b = 0;
    }
    return A;
}

float matrix4x4::minor(int i, int j) {
    matrix3x3 submat = this->submatrix(i, j);
    return submat.det();
}

float matrix4x4::cofactor(int i, int j) {
    float minor = this->minor(i, j);
    if ((i + j) % 2 == 0) {
        return minor;
    } else {
        return -minor;
    }
}

float matrix4x4::det() {
    float determinant = 0;
    for (int j = 0; j < 4; j++) {
        determinant += this->val[j] * this->cofactor(0, j);
    }
    return determinant;
}

matrix4x4 matrix4x4::inverse() {
    float determinant = this->det();
    if (determinant == 0) {
        std::cout << "matrix is not invertible: det = 0.." << std::endl;
        return *this;
    }
    matrix4x4 A;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            A.val[j * 4 + i] = this->cofactor(i, j) / determinant;
        }
    }
    return A;
}

void matrix4x4::print() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << this->val[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
}

namespace mat {
const matrix4x4 identity{{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};

matrix4x4 translation(float x, float y, float z) {
    return matrix4x4{{1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1}};
}

matrix4x4 scaling(float x, float y, float z) {
    return matrix4x4{{x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1}};
}

matrix4x4 rotation_x(float radians) {
    return matrix4x4{{1, 0, 0, 0, 0, cos(radians), -sin(radians), 0, 0, sin(radians), cos(radians), 0, 0, 0, 0, 1}};
}

matrix4x4 rotation_y(float radians) {
    return matrix4x4{{cos(radians), 0, sin(radians), 0, 0, 1, 0, 0, -sin(radians), 0, cos(radians), 0, 0, 0, 0, 1}};
}

matrix4x4 rotation_z(float radians) {
    return matrix4x4{{cos(radians), -sin(radians), 0, 0, sin(radians), cos(radians), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}};
}

matrix4x4 shearing(float xy, float xz, float yx, float yz, float zx, float zy) {
    return matrix4x4({1, xy, xz, 0, yx, 1, yz, 0, zx, zy, 1, 0, 0, 0, 0, 1});
}

matrix4x4 view_transform(vec from, vec to, vec up) {
    vec forward = to - from;
    forward.normalize();
    up.normalize();
    vec left = forward.cross(up);
    vec true_up = left.cross(forward);
    matrix4x4 orientation{{left.x, left.y, left.z, 0, true_up.x, true_up.y, true_up.z, 0, -forward.x, -forward.y, -forward.z, 0, 0, 0, 0, 1}};
    matrix4x4 transl = mat::translation(-from.x, -from.y, -from.z);
    return orientation * transl;
}

} // namespace mat

#endif