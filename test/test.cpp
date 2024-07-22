#include "iostream"

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

int main(void) {
    test_vector();
    return 0;
}