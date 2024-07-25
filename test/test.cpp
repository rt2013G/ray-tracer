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

    matrix4x4 E{{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8}};
    matrix4x4 expected_mat{{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8}};
    matrix4x4 Et = E.transpose();
    if (Et == expected_mat) {
        std::cout << "matrix transpose OK" << std::endl;
    } else {
        std::cout << "matrix transpose NOT OK" << std::endl;
    }

    matrix2x2 F{{1, 5, -3, 2}};
    if (F.det() == 17) {
        std::cout << "matrix2x2 det OK" << std::endl;
    } else {
        std::cout << "matrix2x2 det NOT OK" << std::endl;
    }

    matrix4x4 G{{-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1}};
    matrix3x3 submat = G.submatrix(2, 1);
    matrix3x3 expected_submat{{-6, 1, 6, -8, 8, 6, -7, -1, 1}};
    if (submat == expected_submat) {
        std::cout << "matrix4x4 submat OK" << std::endl;
    } else {
        std::cout << "matrix4x4 submat NOT OK" << std::endl;
    }

    matrix3x3 H{{1, 5, 0, -3, 2, 7, 0, 6, -3}};
    matrix2x2 expected_submat2x2{{-3, 2, 0, 6}};
    matrix2x2 submat2x2 = H.submatrix(0, 2);
    if (submat2x2 == expected_submat2x2) {
        std::cout << "matrix3x3 submat OK" << std::endl;
    } else {
        std::cout << "matrix3x3 submat NOT OK" << std::endl;
    }

    matrix3x3 I{{3, 5, 0, 2, -1, -7, 6, -1, 5}};
    if (I.minor(1, 0) == 25 && I.cofactor(1, 0) == -25 && I.cofactor(0, 0) == -12) {
        std::cout << "matrix3x3 minor and cofactor OK" << std::endl;
    } else {
        std::cout << "matrix3x3 minor and cofactor NOT OK" << std::endl;
    }

    matrix3x3 J{{1, 2, 6, -5, 8, -4, 2, 6, 4}};
    if (J.det() == -196) {
        std::cout << "matrix3x3 det OK" << std::endl;
    } else {
        std::cout << "matrix3x3 det NOT OK" << std::endl;
    }

    matrix4x4 K{{-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9}};
    if (K.det() == -4071) {
        std::cout << "matrix4x4 det OK" << std::endl;
    } else {
        std::cout << "matrix4x4 det NOT OK" << std::endl;
    }

    matrix4x4 L{{8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4}};
    matrix4x4 expected_inverse_L{{-0.15385, -0.15385, -0.28205, -0.53846, -0.07692, 0.12308, 0.02564, 0.03077, 0.35897, 0.35897, 0.43590, 0.92308, -0.69231, -0.69231, -0.76923, -1.92308}};
    if (L.inverse() == expected_inverse_L) {
        std::cout << "matrix4x4 inverse OK" << std::endl;
    } else {
        std::cout << "matrix4x4 inverse NOT OK" << std::endl;
    }

    matrix4x4 M{{3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, -1, 1}};
    matrix4x4 N{{8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5}};
    matrix4x4 N_inv = N.inverse();
    matrix4x4 O = M * N;
    if (O * N_inv == M) {
        std::cout << "matrix4x4 mul inverse OK" << std::endl;
    } else {
        std::cout << "matrix4x4 mul inverse NOT OK" << std::endl;
    }
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