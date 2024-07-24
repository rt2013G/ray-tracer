#include "iostream"

#include "../src/canvas.hpp"
#include "../src/matrix.hpp"
#include "../src/vector.hpp"

void test_vector() {
    vec a{1, 2, 3, 4};
    vec b{2, -3, 4, -5};
    vec c{1, 2, 3, 4};
    vec d = a + b;
    vec e = a - b;

    if (a == c) {
        std::cout << "== OK" << std::endl;
    } else {
        std::cout << "== NOT OK" << std::endl;
    }

    if (d == vec{3, -1, 7, -1}) {
        std::cout << "+ OK" << std::endl;
    } else {
        std::cout << "+ NOT OK" << std::endl;
    }

    if (e == vec{-1, 5, -1, 9}) {
        std::cout << "- OK" << std::endl;
    } else {
        std::cout << "- NOT OK" << std::endl;
    }

    vec f{1, 2, 3, 0};
    if (eq(f.mag(), sqrt(14))) {
        std::cout << "mag() OK" << std::endl;
    } else {
        std::cout << "mag() NOT OK" << std::endl;
    }

    f = f.normalize();
    if (eq(f.x, 0.26726) && eq(f.y, 0.53452) && eq(f.z, 0.80178)) {
        std::cout << "normalize() OK" << std::endl;
    } else {
        std::cout << "normalize() NOT OK" << std::endl;
    }

    vec g{1, 2, 3, 0};
    vec h{2, 3, 4, 0};
    float dot = g.dot(h);
    if (dot == 20) {
        std::cout << "dot() OK" << std::endl;
    } else {
        std::cout << "dot() NOT OK" << std::endl;
    }

    vec cross = g.cross(h);
    if (cross == vec{-1, 2, -1}) {
        std::cout << "cross() OK" << std::endl;
    } else {
        std::cout << "cross() NOT OK" << std::endl;
    }
}

void test_canvas() {
    canvas canv{10, 20};
    for (color &c : canv.pixels) {
        if (c.r == 0 && c.g == 0 && c.b == 0) {
            continue;
        } else {
            std::cout << "canvas NOT OK" << std::endl;
        }
    }

    canv.write(5, 5, color{0.5, 0.3, 0.4});
    color c = canv.get(5, 5);
    if (!eq(c.r, 0.5) || !eq(c.g, 0.3) || !eq(c.b, 0.4)) {
        std::cout << "canvas NOT OK" << std::endl;
        return;
    }

    canv.to_ppm();

    std::cout << "canvas OK" << std::endl;
}

void test_matrices() {
    matrix4x4 A{{1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2}};
    matrix4x4 B{{-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8}};
    matrix4x4 C{{20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42}};

    matrix4x4 res = A * B;
    if (res == C) {
        std::cout << "matrix mul OK" << std::endl;
    } else {
        std::cout << "matrix mul NOT OK" << std::endl;
    }

    matrix4x4 D{{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1}};
    vec b = {1, 2, 3, 1};
    vec Db = D * b;
    vec expected = {18, 24, 33, 1};
    if (Db == expected) {
        std::cout << "matrix * vec OK" << std::endl;
    } else {
        std::cout << "matrix * vec NOT OK" << std::endl;
    }
    std::cout << Db.x << std::endl;
    std::cout << Db.y << std::endl;
    std::cout << Db.z << std::endl;
    std::cout << Db.w << std::endl;
}

int main(void) {
    std::cout << std::endl
              << "testing vectors..." << std::endl;
    test_vector();
    std::cout << std::endl
              << "testing canvas..." << std::endl;
    test_canvas();
    std::cout << std::endl
              << "testing matrices..." << std::endl;
    test_matrices();
    return 0;
}