#include "iostream"

#include "canvas.hpp"
#include "color.hpp"
#include "primitives.hpp"
#include "vector.hpp"

int main(void) {
    const int WIDTH = 100;
    const int HEIGHT = 100;
    canvas canv{WIDTH, HEIGHT};

    sphere s{0};
    matrix4x4 shear = mat::shearing(1, 0, 0, 0, 0, 0);
    matrix4x4 scaling = mat::scaling(0.5, 1, 1);
    s.transform = shear * scaling;
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
            for (intersection &i : inters) {
                if (i.t > 0) {
                    canv.write(x, y, color{1, 0, 0});
                }
            }
        }
    }

    canv.to_ppm();

    return 0;
}