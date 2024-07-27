#ifndef COLOR_HPP
#define COLOR_HPP

#include "vector.hpp"

struct color {
    float r;
    float g;
    float b;
    color(float r, float g, float b);
    color();
    bool operator==(const color &c);
    color operator+(const color &c);
    color operator-(const color &c);
    color operator-();
    color operator*(const float &f);
    color operator*(const color &c);
    void print();
};

color::color(float r, float g, float b) {
    if (r > 1) {
        r = 1;
    } else if (r < 0) {
        r = 0;
    }
    this->r = r;
    if (g > 1) {
        g = 1;
    } else if (g < 0) {
        g = 0;
    }
    this->g = g;
    if (b > 1) {
        b = 1;
    } else if (b < 0) {
        b = 0;
    }
    this->b = b;
}

color::color() {
    this->r = 1;
    this->g = 1;
    this->b = 1;
}

const color BLACK = color(0, 0, 0);
const color WHITE = color(1, 1, 1);
// const color RED = color(1, 0, 0);
// const color GREEN = color(0, 1, 0);
// const color BLUE = color(0, 0, 1);

bool color::operator==(const color &c) {
    if (eq(this->r, c.r) && eq(this->g, c.g) && eq(this->b, c.b)) {
        return true;
    } else {
        return false;
    }
};

color color::operator+(const color &c) {
    return color{
        this->r + c.r,
        this->g + c.g,
        this->b + c.b,
    };
};

color color::operator-(const color &c) {
    return color{
        this->r - c.r,
        this->g - c.g,
        this->b - c.b,
    };
};

color color::operator-() {
    return color{
        -this->r,
        -this->g,
        -this->b,
    };
};

color color::operator*(const float &f) {
    return color{
        this->r * f,
        this->g * f,
        this->b * f,
    };
}

color color::operator*(const color &c) {
    return color{
        this->r * c.r,
        this->g * c.g,
        this->b * c.b,
    };
}

void color::print() {
    std::cout << this->r << " " << this->g << " " << this->b << std::endl;
}

#endif