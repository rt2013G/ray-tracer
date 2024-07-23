#ifndef COLOR_HPP
#define COLOR_HPP

struct color {
    float r;
    float g;
    float b;
    color operator+(const color &c);
    color operator-(const color &c);
    color operator-();
    color operator*(const float &f);
    color operator*(const color &c);
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

#endif