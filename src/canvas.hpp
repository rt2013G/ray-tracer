#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "fstream"
#include "vector"

#include "color.hpp"

struct canvas {
    const int width;
    const int height;
    std::vector<color> pixels;
    canvas(const int width, const int height);
    void write(const int x, const int y, const color &c);
    color get(const int x, const int y);
    void to_ppm();
};

canvas::canvas(const int width, const int height) : width(width), height(height) {
    this->pixels = std::vector<color>(width * height, color{0, 0, 0});
}

void canvas::write(const int x, const int y, const color &c) {
    this->pixels[y * this->width + x] = c;
}

color canvas::get(const int x, const int y) {
    return this->pixels[y * this->width + x];
}

void clamp(int &value) {
    if (value < 0) {
        value = 0;
    }
    if (value > 255) {
        value = 255;
    }
}

void canvas::to_ppm() {
    std::ofstream out("out/canvas.ppm");
    out << "P3" << std::endl;
    out << std::to_string(this->width) << " " << std::to_string(this->height) << std::endl;
    out << "255" << std::endl;
    int count = out.tellp();
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            color c = get(x, y);
            int red = c.r * 255;
            clamp(red);
            out << std::to_string(red);
            int total_count = out.tellp();
            if (total_count - count > 67) {
                out << std::endl;
                count = total_count;
            } else {
                out << " ";
            }

            int green = c.g * 255;
            clamp(green);
            out << std::to_string(green);
            total_count = out.tellp();
            if (total_count - count > 67) {
                out << std::endl;
                count = total_count;
            } else {
                out << " ";
            }

            int blue = c.b * 255;
            clamp(blue);
            out << std::to_string(blue);
            total_count = out.tellp();
            if (total_count - count > 67) {
                out << std::endl;
                count = total_count;
            } else {
                out << " ";
            }
        }
    }
    out << std::endl;
    out.close();
}

#endif