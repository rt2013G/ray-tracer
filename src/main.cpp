#include "iostream"

#include "canvas.hpp"
#include "color.hpp"
#include "primitives.hpp"
#include "vector.hpp"

int main(void) {
    const int WIDTH = 128;
    const int HEIGHT = 128;
    canvas canv{WIDTH, HEIGHT};

    sphere s{0};
    s.mat.surface = color(0.3, 0.5, 0.5);
    matrix4x4 A = mat::scaling(0.3, 0.3, 0.3);
    s.transform = A;
    point_light light{vect::point3(-10, 10, -10), WHITE};
    vec r_origin = vect::point3(0, 0, -5);
    float wall_z = 10;
    float wall_size = 7;
    float pixel_size = wall_size / WIDTH;
    float half = wall_size / 2;

    for (int y = 0; y < HEIGHT; y++) {
        float world_y = half - pixel_size * y;
        for (int x = 0; x < WIDTH; x++) {
            float world_x = -half + pixel_size * x;
            vec position = vect::point3(world_x, world_y, wall_z);
            ray r{r_origin, (position - r_origin).normalize()};
            std::vector<intersection> inters = s.interesect(r);
            intersection i = hit(inters);
            if (i.t >= 0) {
                vec point = r.position(i.t);
                vec normal = s.normal_at(point);
                vec eye = -r.direction;
                eye = eye.normalize();
                color c = phong_lighting(s.mat, point, light, eye, normal);
                canv.write(x, y, c);
            } else {
                canv.write(x, y, BLACK);
            }
        }
    }

    canv.to_ppm();

    return 0;
}