#include "iostream"
#include "string"

#include "../src/canvas.hpp"
#include "../src/matrix.hpp"
#include "../src/primitives.hpp"
#include "../src/vector.hpp"

void PRINT_TEST(bool p, std::string str) {
    if (p) {
        std::cout << str << " OK" << std::endl;
    } else {
        std::cout << str << " ERROR" << std::endl;
    }
}

void test_vector() {
    vec a{1, 2, 3, 4};
    vec b{2, -3, 4, -5};
    vec c{1, 2, 3, 4};
    vec d = a + b;
    vec e = a - b;

    PRINT_TEST(a == c, "vec ==");
    PRINT_TEST(d == vec{3, -1, 7, -1}, "vec +");
    PRINT_TEST(e == vec{-1, 5, -1, 9}, "vec -");

    vec f{1, 2, 3, 0};
    PRINT_TEST(eq(f.mag(), sqrt(14)), "vec mag()");

    f = f.normalize();
    PRINT_TEST(eq(f.x, 0.26726) && eq(f.y, 0.53452) && eq(f.z, 0.80178), "vec normalize()");

    vec g{1, 2, 3, 0};
    vec h{2, 3, 4, 0};
    float dot = g.dot(h);
    PRINT_TEST(dot == 20, "vec dot()");

    vec cross = g.cross(h);
    PRINT_TEST(cross == vec{-1, 2, -1}, "vec cross()");
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
    PRINT_TEST(res == C, "matrix mul");

    matrix4x4 D{{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1}};
    vec b = {1, 2, 3, 1};
    vec Db = D * b;
    vec expected = {18, 24, 33, 1};
    PRINT_TEST(Db == expected, "matrix * vec");

    matrix4x4 E{{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8}};
    matrix4x4 expected_mat{{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8}};
    matrix4x4 Et = E.transpose();
    PRINT_TEST(Et == expected_mat, "matrix transpose");

    matrix2x2 F{{1, 5, -3, 2}};
    PRINT_TEST(F.det() == 17, "matrix2x2 det");

    matrix4x4 G{{-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1}};
    matrix3x3 submat = G.submatrix(2, 1);
    matrix3x3 expected_submat{{-6, 1, 6, -8, 8, 6, -7, -1, 1}};
    PRINT_TEST(submat == expected_submat, "matrix4x4 submat");

    matrix3x3 H{{1, 5, 0, -3, 2, 7, 0, 6, -3}};
    matrix2x2 expected_submat2x2{{-3, 2, 0, 6}};
    matrix2x2 submat2x2 = H.submatrix(0, 2);
    PRINT_TEST(submat2x2 == expected_submat2x2, "matrix3x3 submat");

    matrix3x3 I{{3, 5, 0, 2, -1, -7, 6, -1, 5}};
    PRINT_TEST(I.minor(1, 0) == 25 && I.cofactor(1, 0) == -25 && I.cofactor(0, 0) == -12, "matrix3x3 minor and cofactor");

    matrix3x3 J{{1, 2, 6, -5, 8, -4, 2, 6, 4}};
    PRINT_TEST(J.det() == -196, "matrix3x3 det");

    matrix4x4 K{{-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9}};
    PRINT_TEST(K.det() == -4071, "matrix4x4 det");

    matrix4x4 L{{8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6, -3, 0, -9, -4}};
    matrix4x4 expected_inverse_L{{-0.15385, -0.15385, -0.28205, -0.53846, -0.07692, 0.12308, 0.02564, 0.03077, 0.35897, 0.35897, 0.43590, 0.92308, -0.69231, -0.69231, -0.76923, -1.92308}};
    PRINT_TEST(L.inverse() == expected_inverse_L, "matrix4x4 inverse");

    matrix4x4 M{{3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, -1, 1}};
    matrix4x4 N{{8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5}};
    matrix4x4 N_inv = N.inverse();
    matrix4x4 O = M * N;
    PRINT_TEST(O * N_inv == M, "matrix4x4 mul inverse");
}

void test_transforms() {
    vec v{-3, 4, 5, 0};
    vec p{-3, 4, 5, 1};
    matrix4x4 transform = mat::translation(5, -3, 2);
    PRINT_TEST((v == transform * v) && (transform * p == vec{2, 1, 7, 1}), "mat translation");

    matrix4x4 scal = mat::scaling(2, 3, 4);
    vec p2{-4, 6, 8, 1};
    PRINT_TEST(scal * p2 == vec{-8, 18, 32, 1}, "mat scaling");

    matrix4x4 reflection = mat::scaling(-1, 1, 1);
    PRINT_TEST(reflection * p2 == vec{4, 6, 8, 1}, "mat reflection");

    vec px{0, 1, 0, 1};
    vec py{0, 0, 1, 1};
    vec pz{0, 1, 0, 1};
    matrix4x4 rot_x = mat::rotation_x(M_PI / 4);
    matrix4x4 rot_y = mat::rotation_y(M_PI / 4);
    matrix4x4 rot_z = mat::rotation_z(M_PI / 4);
    float sqrt2 = sqrt(2);
    PRINT_TEST(rot_x * px == vec{0, sqrt2 / 2, sqrt2 / 2, 1} &&
                   rot_y * py == vec{sqrt2 / 2, 0, sqrt2 / 2, 1} &&
                   rot_z * pz == vec{-sqrt2 / 2, sqrt2 / 2, 0, 1},
               "mat rotation");

    matrix4x4 shxy = mat::shearing(1, 0, 0, 0, 0, 0);
    matrix4x4 shzx = mat::shearing(0, 0, 0, 0, 1, 0);
    vec ps{2, 3, 4, 1};
    PRINT_TEST(shxy * ps == vec{5, 3, 4, 1} && shzx * ps == vec{2, 3, 6, 1}, "mat shearing");

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
    PRINT_TEST(point_rst == expected_point && point_t == expected_point, "mat chained transforms");
}

void test_rays() {
    ray r{vect::point3(2, 3, 4),
          vect::vector3(1, 0, 0)};
    vec p2 = r.position(2.5);
    PRINT_TEST(p2 == vect::point3(4.5, 3, 4), "ray position");

    object s{};
    ray r2{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters = intersect_sphere(s, r2);
    PRINT_TEST(inters.size() == 2 && eq(inters[0].t, 4.0) && eq(inters[1].t, 6.0), "ray sphere intersection 1");

    ray r3{vect::point3(0, 1, -5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters2 = intersect_sphere(s, r3);
    PRINT_TEST(inters2.size() == 2 && eq(inters2[0].t, 5.0) && eq(inters2[1].t, 5.0), "ray sphere intersection 2");

    ray r4{vect::point3(0, 2, -5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters3 = intersect_sphere(s, r4);
    PRINT_TEST(inters3.size() == 0, "ray sphere intersection 3");

    ray r5{vect::point3(0, 0, 0), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters4 = intersect_sphere(s, r5);
    PRINT_TEST(inters4.size() == 2 && eq(inters4[0].t, -1) && eq(inters4[1].t, 1), "ray sphere intersection 4");

    ray r6{vect::point3(0, 0, 5), vect::vector3(0, 0, 1)};
    std::vector<intersection> inters5 = intersect_sphere(s, r6);
    PRINT_TEST(inters5.size() == 2 && eq(inters5[0].t, -6) && eq(inters5[1].t, -4), "ray sphere intersection 5");
}

void test_hits() {
    object s{};
    std::vector<intersection> inters{intersection{-1, s}, intersection{1, s}};
    PRINT_TEST(hit(inters).t == 1, "hit 1");

    inters = {intersection{5, s}, intersection{7, s}, intersection{-3, s}, intersection{2, s}};
    PRINT_TEST(hit(inters).t == 2, "hit 2");

    ray r{vect::point3(1, 2, 3), vect::vector3(0, 1, 0)};
    ray r_trans = r.transform(mat::translation(3, 4, 5));
    PRINT_TEST(r_trans.origin == vect::point3(4, 6, 8) && r_trans.direction == vect::vector3(0, 1, 0), "ray transform 1");

    r_trans = r.transform(mat::scaling(2, 3, 4));
    PRINT_TEST(r_trans.origin == vect::point3(2, 6, 12) && r_trans.direction == vect::vector3(0, 3, 0), "ray transform 2");

    object s1{};
    ray r1{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    s1.transform = mat::scaling(2, 2, 2);
    inters = intersect_sphere(s1, r1);
    PRINT_TEST(inters.size() == 2 && inters[0].t == 3 && inters[1].t == 7, "scaled sphere intersect");

    s1.transform = mat::translation(5, 0, 0);
    inters = intersect_sphere(s1, r1);
    PRINT_TEST(inters.size() == 0, "translated sphere intersect");
}

void test_normals() {
    object s{};
    vec n = normal_at(s, vect::point3(1, 0, 0));
    PRINT_TEST(n == vect::vector3(1, 0, 0), "normal 1");

    n = normal_at(s, vect::point3(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3));
    PRINT_TEST(n == vect::vector3(sqrt(3) / 3, sqrt(3) / 3, sqrt(3) / 3), "normal 2");

    s.transform = mat::translation(0, 1, 0);
    n = normal_at(s, vect::point3(0, 1.70711, -0.70711));
    PRINT_TEST(n == vect::vector3(0, 0.70711, -0.70711), "translated normal");

    matrix4x4 A = mat::scaling(1, 0.5, 1);
    matrix4x4 B = mat::rotation_z(M_PI / 5);
    s.transform = A * B;
    n = normal_at(s, vect::point3(0, sqrt(2) / 2, -sqrt(2) / 2));
    PRINT_TEST(n == vect::vector3(0, 0.97014, -0.24254), "translated normal 2");

    vec v = vect::vector3(1, -1, 0);
    n = vect::vector3(0, 1, 0);
    vec r = vect::reflect(v, n);
    PRINT_TEST(r == vect::vector3(1, 1, 0), "reflect");

    v = vect::vector3(0, -1, 0);
    n = vect::vector3(sqrt(2) / 2, sqrt(2) / 2, 0);
    r = vect::reflect(v, n);
    PRINT_TEST(r == vect::vector3(1, 0, 0), "reflect 2");
}

void test_lighting() {
    material m{};
    vec position = vect::origin;

    vec eye = vect::vector3(0, 0, -1);
    vec normal = vect::vector3(0, 0, -1);
    point_light light{vect::point3(0, 0, -10), WHITE};
    PRINT_TEST(phong_lighting(m, position, light, eye, normal) == color{1.9, 1.9, 1.9}, "lighting");

    eye = vect::vector3(0, sqrt(2) / 2, -sqrt(2) / 2);
    PRINT_TEST(phong_lighting(m, position, light, eye, normal) == WHITE, "lighting 2");

    eye = vect::vector3(0, 0, -1);
    light = {vect::point3(0, 10, -10), WHITE};
    PRINT_TEST(phong_lighting(m, position, light, eye, normal) == color(0.7364, 0.7364, 0.7364), "lighting 3");

    eye = vect::vector3(0, -sqrt(2) / 2, -sqrt(2) / 2);
    normal = vect::vector3(0, 0, -1);
    PRINT_TEST(phong_lighting(m, position, light, eye, normal) == color(1.6364, 1.6364, 1.6364), "lighting 4");

    eye = vect::vector3(0, 0, -1);
    light = {vect::point3(0, 0, 10), WHITE};
    PRINT_TEST(phong_lighting(m, position, light, eye, normal) == color(0.1, 0.1, 0.1), "lighting 5");
}

void test_computations() {
    ray r{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    object sphere{};
    intersection i{4, sphere};
    computation comp{i, r};
    PRINT_TEST(comp.point == vect::point3(0, 0, -1) && comp.eye_vec == vect::vector3(0, 0, -1) && comp.normal_vec == vect::vector3(0, 0, -1) && comp.inside == false, "prepare computations");
}

void test_world() {
    world w{};
    std::vector<intersection> inters = w.intersect(ray{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)});
    PRINT_TEST(inters.size() == 4 && inters[0].t == 4 && inters[1].t == 4.5 && inters[2].t == 5.5 && inters[3].t == 6, "world");

    ray r{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    object obj = w.objects[0];
    intersection i{4, obj};
    computation comp{i, r};
    PRINT_TEST(w.shade_hit(comp) == color(0.38066, 0.47583, 0.2855), "shade hit 1");

    w.plights[0] = point_light{vect::point3(0, 0.25, 0), WHITE};
    r.origin = vect::point3(0, 0, 0);
    obj = w.objects[1];
    i.t = 0.5;
    i.obj = obj;
    computation comp2{i, r};
    PRINT_TEST(w.shade_hit(comp2) == color(0.90498, 0.90498, 0.90498), "shade hit 2");

    world w2{};
    r = ray{vect::point3(0, 0, -5), vect::vector3(0, 1, 0)};
    PRINT_TEST(w2.color_at(r) == BLACK, "color at 1");

    r = ray{vect::point3(0, 0, -5), vect::vector3(0, 0, 1)};
    PRINT_TEST(w2.color_at(r) == color{0.38066, 0.47583, 0.2855}, "color at 2");

    w2.objects[0].mat.ambient = 1;
    w2.objects[1].mat.ambient = 1;
    r = ray{vect::point3(0, 0, 0.75), vect::vector3(0, 0, -1)};
    PRINT_TEST(w2.color_at(r) == w2.objects[1].mat.surface, "color at 3");

    matrix4x4 view = mat::view_transform(vect::point3(0, 0, 0), vect::point3(0, 0, -1), vect::vector3(0, 1, 0));
    PRINT_TEST(view == mat::identity, "view transform 1");

    view = mat::view_transform(vect::point3(0, 0, 0), vect::point3(0, 0, 1), vect::vector3(0, 1, 0));
    PRINT_TEST(view == mat::scaling(-1, 1, -1), "view transform 2");

    view = mat::view_transform(vect::point3(0, 0, 8), vect::point3(0, 0, 0), vect::vector3(0, 1, 0));
    PRINT_TEST(view == mat::translation(0, 0, -8), "view transform 3");

    view = mat::view_transform(vect::point3(1, 3, 2), vect::point3(4, -2, 8), vect::vector3(1, 1, 0));
    PRINT_TEST(view == matrix4x4{{-0.50709, 0.50709, 0.67612, -2.36643, 0.76772, 0.60609, 0.12122, -2.82843, -0.35857, 0.59761, -0.71714, 0.00000, 0.00000, 0.00000, 0.00000, 1.00000}}, "view transform 4");
}

void test_camera() {
    camera c(200, 125, M_PI / 2);
    PRINT_TEST(eq(c.pixel_size, 0.01), "camera 1");

    c = camera(125, 200, M_PI / 2);
    PRINT_TEST(eq(c.pixel_size, 0.01), "camera 2");

    c = camera(201, 101, M_PI / 2);
    ray r = c.ray_for_pixel(100, 50);
    PRINT_TEST(r.origin == vect::point3(0, 0, 0) && r.direction == vect::vector3(0, 0, -1), "camera ray for pixel 1");

    r = c.ray_for_pixel(0, 0);
    PRINT_TEST(r.origin == vect::point3(0, 0, 0) && r.direction == vect::vector3(0.66519, 0.33259, -0.66851), "camera ray for pixel 2");

    matrix4x4 A = mat::rotation_y(M_PI / 4);
    matrix4x4 B = mat::translation(0, -2, 5);
    c.transform = A * B;
    r = c.ray_for_pixel(100, 50);
    PRINT_TEST(r.origin == vect::point3(0, 2, -5) && r.direction == vect::vector3(sqrt(2) / 2, 0, -sqrt(2) / 2), "camera ray for pixel 3");

    world w{};
    c = camera(11, 11, M_PI / 2, mat::view_transform(vect::point3(0, 0, -5), vect::point3(0, 0, 0), vect::vector3(0, 1, 0)));
    canvas image = c.render(w);
    PRINT_TEST(image.get(5, 5) == color(0.38066, 0.47583, 0.2855), "camera render");
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
    std::cout << std::endl
              << "testing computations..." << std::endl;
    test_computations();
    std::cout << std::endl
              << "testing world..." << std::endl;
    test_world();
    std::cout << std::endl
              << "testing camera..." << std::endl;
    test_camera();
    return 0;
}