#include "iostream"

#include "../src/canvas.hpp"
#include "../src/matrix.hpp"
#include "../src/primitives.hpp"
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
        std::cout << "== ERROR" << std::endl;
    }

    if (d == vec{3, -1, 7, -1}) {
        std::cout << "+ OK" << std::endl;
    } else {
        std::cout << "+ ERROR" << std::endl;
    }

    if (e == vec{-1, 5, -1, 9}) {
        std::cout << "- OK" << std::endl;
    } else {
        std::cout << "- ERROR" << std::endl;
    }

    vec f{1, 2, 3, 0};
    if (eq(f.mag(), sqrt(14))) {
        std::cout << "mag() OK" << std::endl;
    } else {
        std::cout << "mag() ERROR" << std::endl;
    }

    f = f.normalize();
    if (eq(f.x, 0.26726) && eq(f.y, 0.53452) && eq(f.z, 0.80178)) {
        std::cout << "normalize() OK" << std::endl;
    } else {
        std::cout << "normalize() ERROR" << std::endl;
    }

    vec g{1, 2, 3, 0};
    vec h{2, 3, 4, 0};
    float dot = g.dot(h);
    if (dot == 20) {
        std::cout << "dot() OK" << std::endl;
    } else {
        std::cout << "dot() ERROR" << std::endl;
    }

    vec cross = g.cross(h);
    if (cross == vec{-1, 2, -1}) {
        std::cout << "cross() OK" << std::endl;
    } else {
        std::cout << "cross() ERROR" << std::endl;
    }
}

void test_canvas() {
    canvas canv{10, 20};
    for (color &c : canv.pixels) {
        if (c.r == 0 && c.g == 0 && c.b == 0) {
            continue;
        } else {
            std::cout << "canvas ERROR" << std::endl;
        }
    }

    canv.write(5, 5, color{0.5, 0.3, 0.4});
    color c = canv.get(5, 5);
    if (!eq(c.r, 0.5) || !eq(c.g, 0.3) || !eq(c.b, 0.4)) {
        std::cout << "canvas ERROR" << std::endl;
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
        std::cout << "matrix mul ERROR" << std::endl;
    }

    matrix4x4 D{{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1}};
    vec b = {1, 2, 3, 1};
    vec Db = D * b;
    vec expected = {18, 24, 33, 1};
    if (Db == expected) {
        std::cout << "matrix * vec OK" << std::endl;
    } else {
        std::cout << "matrix * vec ERROR" << std::endl;
    }

    matrix4x4 E{{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8}};
    matrix4x4 expected_mat{{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8}};
    matrix4x4 Et = E.transpose();
    if (Et == expected_mat) {
        std::cout << "matrix transpose OK" << std::endl;
    } else {
        std::cout << "matrix transpose ERROR" << std::endl;
    }

    matrix2x2 F{{1, 5, -3, 2}};
    if (F.det() == 17) {
        std::cout << "matrix2x2 det OK" << std::endl;
    } else {
        std::cout << "matrix2x2 det ERROR" << std::endl;
    }

    matrix4x4 G{{-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1}};
    matrix3x3 submat = G.submatrix(2, 1);
    matrix3x3 expected_submat{{-6, 1, 6, -8, 8, 6, -7, -1, 1}};
    if (submat == expected_submat) {
        std::cout << "matrix4x4 submat OK" << std::endl;
    } else {
        std::cout << "matrix4x4 submat ERROR" << std::endl;
    }

    matrix3x3 H{{1, 5, 0, -3, 2, 7, 0, 6, -3}};
    matrix2x2 expected_submat2x2{{-3, 2, 0, 6}};
    matrix2x2 submat2x2 = H.submatrix(0, 2);
    if (submat2x2 == expected_submat2x2) {
        std::cout << "matrix3x3 submat OK" << std::endl;
    } else {
        std::cout << "matrix3x3 submat ERROR" << std::endl;
    }

    matrix3x3 I{{3, 5, 0, 2, -1, -7, 6, -1, 5}};
    if (I.minor(1, 0) == 25 && I.cofactor(1, 0) == -25 && I.cofactor(0, 0) == -12) {
        std::cout << "matrix3x3 minor and cofactor OK" << std::endl;
    } else {
        std::cout << "matrix3x3 minor and cofactor ERROR" << std::endl;
    }

    matrix3x3 J{{1, 2, 6, -5, 8, -4, 2, 6, 4}};
    if (J.det() == -196) {
        std::cout << "matrix3x3 det OK" << std::endl;
    } else {
        std::cout << "matrix3x3 det ERROR" << std::endl;
    }

    matrix4x4 K{{-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9}};
    if (K.det() == -4071) {
        std::cout << "matrix4x4 det OK" << std::endl;
    } else {
        std::cout << "matrix4x4 det ERROR" << std::endl;
    }

    matrix4x4 L{{8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4}};
    matrix4x4 expected_inverse_L{{-0.15385, -0.15385, -0.28205, -0.53846, -0.07692, 0.12308, 0.02564, 0.03077, 0.35897, 0.35897, 0.43590, 0.92308, -0.69231, -0.69231, -0.76923, -1.92308}};
    if (L.inverse() == expected_inverse_L) {
        std::cout << "matrix4x4 inverse OK" << std::endl;
    } else {
        std::cout << "matrix4x4 inverse ERROR" << std::endl;
    }

    matrix4x4 M{{3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, -1, 1}};
    matrix4x4 N{{8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5}};
    matrix4x4 N_inv = N.inverse();
    matrix4x4 O = M * N;
    if (O * N_inv == M) {
        std::cout << "matrix4x4 mul inverse OK" << std::endl;
    } else {
        std::cout << "matrix4x4 mul inverse ERROR" << std::endl;
    }
}

void test_transforms() {
    vec v{-3, 4, 5, 0};
    vec p{-3, 4, 5, 1};
    matrix4x4 transform = mat::translation(5, -3, 2);
    if ((v == transform * v) && (transform * p == vec{2, 1, 7, 1})) {
        std::cout << "mat translation OK" << std::endl;
    } else {
        std::cout << "mat translation ERROR" << std::endl;
    }

    matrix4x4 scal = mat::scaling(2, 3, 4);
    vec p2{-4, 6, 8, 1};
    if (scal * p2 == vec{-8, 18, 32, 1}) {
        std::cout << "mat scaling OK" << std::endl;
    } else {
        std::cout << "mat scaling ERROR" << std::endl;
    }

    matrix4x4 reflection = mat::scaling(-1, 1, 1);
    if (reflection * p2 == vec{4, 6, 8, 1}) {
        std::cout << "mat reflection OK" << std::endl;
    } else {
        std::cout << "mat reflection ERROR" << std::endl;
    }

    vec px{0, 1, 0, 1};
    vec py{0, 0, 1, 1};
    vec pz{0, 1, 0, 1};
    matrix4x4 rot_x = mat::rotation_x(M_PI / 4);
    matrix4x4 rot_y = mat::rotation_y(M_PI / 4);
    matrix4x4 rot_z = mat::rotation_z(M_PI / 4);
    float sqrt2 = sqrt(2);
    if (rot_x * px == vec{0, sqrt2 / 2, sqrt2 / 2, 1} &&
        rot_y * py == vec{sqrt2 / 2, 0, sqrt2 / 2, 1} &&
        rot_z * pz == vec{-sqrt2 / 2, sqrt2 / 2, 0, 1}) {
        std::cout << "mat rotation x OK" << std::endl;
    } else {
        std::cout << "mat rotation x ERROR" << std::endl;
    }

    matrix4x4 shxy = mat::shearing(1, 0, 0, 0, 0, 0);
    matrix4x4 shzx = mat::shearing(0, 0, 0, 0, 1, 0);
    vec ps{2, 3, 4, 1};
    if (shxy * ps == vec{5, 3, 4, 1} && shzx * ps == vec{2, 3, 6, 1}) {
        std::cout << "mat shearing OK" << std::endl;
    } else {
        std::cout << "mat shearing ERROR" << std::endl;
    }

    vec point{1, 0, 1, 1};
    matrix4x4 A = mat::rotation_x(M_PI / 2);
    vec point_r = A * point;
    matrix4x4 B = mat::scaling(5, 5, 5);
    vec point_rs = B * point_r;
    matrix4x4 C = mat::translation(10, 5, 7);
    vec point_rst = C * point_rs;

    matrix4x4 T = C * B * A;
    vec point_t = T * point;
    vec expected_point = vec{15, 0, 7, 1};
    if (point_rst == expected_point && point_t == expected_point) {
        std::cout << "mat chained transforms OK" << std::endl;
    } else {
        std::cout << "mat chained transforms ERROR" << std::endl;
    }
}

void test_rays() {
    ray r{vect::point3(2, 3, 4),
          vect::vector3(1, 0, 0)};

    vec p2 = r.position(2.5);
    if (p2 == vect::point3(4.5, 3, 4)) {
        std::cout << "rays position OK" << std::endl;
    } else {
        std::cout << "rays position ERROR";
    }

    sphere s{0};
    ray r2{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters = s.interesect(r2);
    if (inters.size() == 2 && eq(inters[0].t, 4.0) && eq(inters[1].t, 6.0)) {
        std::cout << "ray sphere intersection 1 OK" << std::endl;
    } else {
        std::cout << "ray sphere intersection 1 ERROR" << std::endl;
    }

    ray r3{vect::point3(0, 1, -5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters2 = s.interesect(r3);
    if (inters2.size() == 2 && eq(inters2[0].t, 5.0) && eq(inters2[1].t, 5.0)) {
        std::cout << "ray sphere intersection 2 OK" << std::endl;
    } else {
        std::cout << "ray sphere intersection 2 ERROR" << std::endl;
    }

    ray r4{vect::point3(0, 2, -5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters3 = s.interesect(r4);
    if (inters3.size() == 0) {
        std::cout << "ray sphere intersection 3 OK" << std::endl;
    } else {
        std::cout << "ray sphere intersection 3 ERROR" << std::endl;
    }

    ray r5{vect::point3(0, 0, 0), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters4 = s.interesect(r5);
    if (inters4.size() == 2 && eq(inters4[0].t, -1) && eq(inters4[1].t, 1)) {
        std::cout << "ray sphere intersection 4 OK" << std::endl;
    } else {
        std::cout << "ray sphere intersection 4 ERROR" << std::endl;
    }

    ray r6{vect::point3(0, 0, 5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters5 = s.interesect(r6);
    if (inters5.size() == 2 && eq(inters5[0].t, -6) && eq(inters5[1].t, -4)) {
        std::cout << "ray sphere intersection 5 OK" << std::endl;
    } else {
        std::cout << "ray sphere intersection 5 ERROR" << std::endl;
    }
}

void test_hits() {
    sphere s{0};
    std::vector<intersection> inters{intersection{-1, s}, intersection{1, s}};
    if (hit(inters).t == 1) {
        std::cout << "hit 1 OK" << std::endl;
    } else {
        std::cout << "hit 2 OK" << std::endl;
    }

    inters = {intersection{5, s}, intersection{7, s}, intersection{-3, s}, intersection{2, s}};
    if (hit(inters).t == 2) {
        std::cout << "hit 2 OK" << std::endl;
    } else {
        std::cout << "hit 2 OK" << std::endl;
    }

    ray r{vect::point3(1, 2, 3), vect::vector3(0, 1, 0)};
    ray r_trans = r.transform(mat::translation(3, 4, 5));
    if (r_trans.origin == vect::point3(4, 6, 8) && r_trans.direction == vect::vector3(0, 1, 0)) {
        std::cout << "ray transform 1 OK" << std::endl;
    } else {
        std::cout << "ray transform 1 ERROR" << std::endl;
    }
    r_trans = r.transform(mat::scaling(2, 3, 4));
    if (r_trans.origin == vect::point3(2, 6, 12) && r_trans.direction == vect::vector3(0, 3, 0)) {
        std::cout << "ray transform 2 OK" << std::endl;
    } else {
        std::cout << "ray transform 2 ERROR" << std::endl;
    }

    sphere s1{1};
    ray r1{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    s1.transform = mat::scaling(2, 2, 2);
    inters = s1.interesect(r1);
    if (inters.size() == 2 && inters[0].t == 3 && inters[1].t == 7) {
        std::cout << "scaled sphere intersect OK" << std::endl;
    } else {
        std::cout << "scaled sphere intersect ERROR" << std::endl;
    }

    s1.transform = mat::translation(5, 0, 0);
    inters = s1.interesect(r1);
    if (inters.size() == 0) {
        std::cout << "translated sphere intersect OK" << std::endl;
    } else {
        std::cout << "translated sphere intersect ERROR" << std::endl;
    }
}

void test_normals() {
    sphere s{0};
    vec n = s.normal_at(vect::point3(1, 0, 0));
    if (n == vect::vector3(1, 0, 0)) {
        std::cout << "normal 1 OK" << std::endl;
    } else {
        std::cout << "normal 1 ERROR" << std::endl;
    }

    n = s.normal_at(vect::point3(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    if (n == vect::vector3(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3)) {
        std::cout << "normal 2 OK" << std::endl;
    } else {
        std::cout << "normal 2 ERROR" << std::endl;
    }

    s.transform = mat::translation(0, 1, 0);
    n = s.normal_at(vect::point3(0, 1.70711, -0.70711));
    if (n == vect::vector3(0, 0.70711, -0.70711)) {
        std::cout << "translated normal OK" << std::endl;
    } else {
        std::cout << "translated normal ERROR" << std::endl;
    }

    matrix4x4 A = mat::scaling(1, 0.5, 1);
    matrix4x4 B = mat::rotation_z(M_PI / 5);
    s.transform = A * B;
    n = s.normal_at(vect::point3(0, sqrt(2) / 2, -sqrt(2) / 2));
    if (n == vect::vector3(0, 0.97014, -0.24254)) {
        std::cout << "translated normal 2 OK" << std::endl;
    } else {
        std::cout << "translated normal 2 ERROR" << std::endl;
    }

    vec v = vect::vector3(1, -1, 0);
    n = vect::vector3(0, 1, 0);
    vec r = vect::reflect(v, n);
    if (r == vect::vector3(1, 1, 0)) {
        std::cout << "reflect OK" << std::endl;
    } else {
        std::cout << "reflect ERROR" << std::endl;
    }

    v = vect::vector3(0, -1, 0);
    n = vect::vector3(sqrt(2) / 2, sqrt(2) / 2, 0);
    r = vect::reflect(v, n);
    if (r == vect::vector3(1, 0, 0)) {
        std::cout << "reflect 2 OK" << std::endl;
    } else {
        std::cout << "reflect 2 ERROR" << std::endl;
    }
}

void test_lighting() {
    material m{};
    vec position = vect::origin;

    vec eye = vect::vector3(0, 0, -1);
    vec normal = vect::vector3(0, 0, -1);
    point_light light{vect::point3(0, 0, -10), WHITE};
    if (phong_lighting(m, position, light, eye, normal) == color{1.9, 1.9, 1.9}) {
        std::cout << "lighting OK" << std::endl;
    } else {
        std::cout << "lighting ERROR" << std::endl;
    }

    eye = vect::vector3(0, sqrt(2) / 2, -sqrt(2) / 2);
    if (phong_lighting(m, position, light, eye, normal) == WHITE) {
        std::cout << "lighting 2 OK" << std::endl;
    } else {
        std::cout << "lighting 2 ERROR" << std::endl;
    }

    eye = vect::vector3(0, 0, -1);
    light = {vect::point3(0, 10, -10), WHITE};
    if (phong_lighting(m, position, light, eye, normal) == color(0.7364, 0.7364, 0.7364)) {
        std::cout << "lighting 3 OK" << std::endl;
    } else {
        std::cout << "lighting 3 ERROR" << std::endl;
    }

    eye = vect::vector3(0, -sqrt(2) / 2, -sqrt(2) / 2);
    normal = vect::vector3(0, 0, -1);
    if (phong_lighting(m, position, light, eye, normal) == color(1.6364, 1.6364, 1.6364)) {
        std::cout << "lighting 4 OK" << std::endl;
    } else {
        std::cout << "lighting 4 ERROR" << std::endl;
    }

    eye = vect::vector3(0, 0, -1);
    light = {vect::point3(0, 0, 10), WHITE};
    if (phong_lighting(m, position, light, eye, normal) == color(0.1, 0.1, 0.1)) {
        std::cout << "lighting 5 OK" << std::endl;
    } else {
        std::cout << "lighting 5 ERROR" << std::endl;
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
    std::cout << std::endl
              << "testing transforms..." << std::endl;
    test_transforms();
    std::cout << std::endl
              << "testing rays..." << std::endl;
    test_rays();
    std::cout << std::endl
              << "testing hits..." << std::endl;
    test_hits();
    std::cout << std::endl
              << "testing normals..." << std::endl;
    test_normals();
    std::cout << std::endl
              << "testing lighting..." << std::endl;
    test_lighting();
    return 0;
}