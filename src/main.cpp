#include "iostream"

#include "canvas.hpp"
#include "color.hpp"
#include "lighting.hpp"
#include "object.hpp"
#include "vector.hpp"
#include "world.hpp"

static const int WIDTH = 256;
static const int HEIGHT = 256;

void demo_snowman() {
    object floor = object(mat::scaling(10, 0.01, 10));
    floor.mat.surface = color(0.4, 1, 0.4);

    matrix4x4 A = mat::translation(0, 0, 5);
    matrix4x4 B = mat::rotation_y(-M_PI / 4);
    matrix4x4 C = mat::rotation_x(M_PI / 2);
    matrix4x4 D = mat::scaling(10, 0.01, 10);
    object left_w = object(A * B * C * D);
    left_w.mat.surface = color(0.7, 0.8, 1);
    left_w.mat.shininess = 16;

    B = mat::rotation_y(M_PI / 4);
    object right_w = object(A * B * C * D);
    right_w.mat = left_w.mat;

    object base = object(mat::translation(0, 0.5, 0));
    base.mat.surface = WHITE;

    A = mat::translation(0, 1.75, 0);
    B = mat::scaling(0.6, 0.6, 0.6);
    object middle = object(A * B);
    middle.mat.surface = WHITE;

    A = mat::translation(0.17, 1.95, -0.8);
    B = mat::scaling(0.06, 0.06, 0.06);
    object left_eye = object(A * B);
    left_eye.mat.surface = BLACK;

    A = mat::translation(-0.17, 1.95, -0.8);
    object right_eye = object(A * B);
    right_eye.mat = left_eye.mat;

    A = mat::translation(0, 1.75, -0.8);
    object nose = object(A * B);
    nose.mat.surface = color(1, 0.65, 0);
    nose.mat.diffuse = 1;
    nose.mat.specular = 0;

    point_light plight{vec::point3(-10, 10, -10), WHITE};
    std::vector<point_light> lights{plight};
    std::vector<object> objects{floor, left_w, right_w, base, middle, left_eye, right_eye, nose};
    world w(lights, objects);

    camera c(WIDTH, HEIGHT, M_PI / 3);
    c.transform = mat::view_transform(vec::point3(0, 1.5, -5), vec::point3(0, 1, 0), vec::vector3(0, 1, 0));

    canvas image = c.render(w);
    image.to_ppm();
}

void demo() {
    object floor = object(mat::scaling(10, 0.01, 10));
    floor.mat.surface = color(1, 0.9, 0.9);
    floor.mat.specular = 0;

    matrix4x4 A = mat::translation(0, 0, 5);
    matrix4x4 B = mat::rotation_y(-M_PI / 4);
    matrix4x4 C = mat::rotation_x(M_PI / 2);
    matrix4x4 D = mat::scaling(10, 0.01, 10);
    object left_w = object(A * B * C * D);
    left_w.mat = floor.mat;

    B = mat::rotation_y(M_PI / 4);
    object right_w = object(A * B * C * D);
    right_w.mat = left_w.mat;

    object middle = object(mat::translation(-0.5, 1, 0.5));
    middle.mat.surface = color(0.1, 1, 0.5);
    middle.mat.diffuse = 0.7;
    middle.mat.specular = 0.3;

    A = mat::translation(1.5, 0.5, -0.5);
    B = mat::scaling(0.5, 0.5, 0.5);
    object right = object(A * B);
    right.mat.surface = color(0.5, 1, 0.1);
    right.mat.diffuse = 0.7;
    right.mat.specular = 0.3;

    A = mat::translation(-1.5, 0.33, -0.75);
    B = mat::scaling(0.33, 0.33, 0.33);
    object left = object(A * B);
    left.mat.surface = color(1, 0.8, 0.1);
    left.mat.diffuse = 0.7;
    left.mat.specular = 0.3;

    point_light plight{vec::point3(-10, 10, -10), WHITE};
    std::vector<point_light> lights{plight};
    std::vector<object> objects{floor, left_w, right_w, middle, right, left};
    world w(lights, objects);

    camera c(WIDTH, HEIGHT, M_PI / 3);
    c.transform = mat::view_transform(vec::point3(0, 1.5, -5), vec::point3(0, 1, 0), vec::vector3(0, 1, 0));

    canvas image = c.render(w);
    image.to_ppm();
}

int main(void) {
    // demo_snowman();
    demo();

    return 0;
}