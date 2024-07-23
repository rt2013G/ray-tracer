#include "iostream"

#include "canvas.hpp"
#include "color.hpp"
#include "vector.hpp"

int main(void) {
    const int WIDTH = 1000;
    const int HEIGHT = 500;
    canvas canv{WIDTH, HEIGHT};
    vec pos{0, 1, 0, 1};
    vec velocity{1, 1.8, 0, 0};
    velocity.normalize();
    velocity = velocity * 11.25;
    vec gravity{0, -0.1, 0, 0};
    vec wind{-0.01, 0, 0, 0};

    int i = 0;
    while (pos.y >= 0) {
        i++;
        pos = pos + velocity;
        velocity = velocity + gravity + wind;

        int canv_y = HEIGHT - pos.y;
        if (pos.x < 0 || pos.x >= WIDTH || canv_y < 0 || canv_y >= HEIGHT) {
            continue;
        } else {
            canv.write(int(pos.x), canv_y, color{255, 255, 255});
        }
    }

    canv.to_ppm();

    std::cout << "X: " << pos.x << std::endl
              << "Y: " << pos.y << std::endl;
    std::cout << "TICK COUNT: " << i << std::endl;

    return 0;
}