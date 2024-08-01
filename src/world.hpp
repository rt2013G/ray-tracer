
#ifndef WORLD_HPP
#define WORLD_HPP

#include "vector"

#include "lighting.hpp"
#include "object.hpp"

struct world {
    std::vector<point_light> plights;
    std::vector<object> objects;
    world();
    world(std::vector<point_light> plights, std::vector<object> objects);
    std::vector<ray::intersection> intersect(ray::ray r);
    bool is_shadowed(point_light light, vector point);
    color shade_hit(ray::computation comp);
    color color_at(ray::ray r);
};

world::world() {
    this->plights.push_back(point_light{vec::point3(-10, 10, -10), WHITE});
    object s1{};
    s1.mat.surface = color(0.8, 1, 0.6);
    s1.mat.diffuse = 0.7;
    s1.mat.specular = 0.2;
    object s2{};
    s2.transform = mat::scaling(0.5, 0.5, 0.5);
    this->objects.push_back(s1);
    this->objects.push_back(s2);
}

world::world(std::vector<point_light> plights, std::vector<object> objects) {
    this->plights = plights;
    this->objects = objects;
}

bool interesection_sorter(ray::intersection &i1, ray::intersection &i2) {
    return i1.t < i2.t;
}

std::vector<ray::intersection> world::intersect(ray::ray r) {
    std::vector<ray::intersection> inters{};
    for (object &obj : this->objects) {
        std::vector<ray::intersection> result = intersect_sphere(obj, r);
        for (ray::intersection &i : result) {
            inters.push_back(i);
        }
    }
    std::sort(inters.begin(), inters.end(), interesection_sorter);
    return inters;
}

bool world::is_shadowed(point_light light, vector point) {
    vector v = light.position - point;
    float distance = v.mag();
    vector direction = v.normalize();

    ray::ray r{point, direction};
    std::vector<ray::intersection> inters = this->intersect(r);
    ray::intersection i = ray::hit(inters);
    if (i.t < 0) {
        return false;
    }
    if (i.t > 0 && i.t < distance) {
        return true;
    }
    return false;
}

color world::shade_hit(ray::computation comp) {
    color c = BLACK;
    for (point_light &pl : this->plights) {
        c = c + phong_lighting(comp.obj.mat, comp.over_point, pl,
                               comp.eye_vec, comp.normal_vec,
                               this->is_shadowed(pl, comp.over_point));
    }
    return c;
}

color world::color_at(ray::ray r) {
    std::vector<ray::intersection> inters = this->intersect(r);
    ray::intersection i = hit(inters);
    if (i.t < 0) {
        return BLACK;
    }
    ray::computation comp{i, r};
    return this->shade_hit(comp);
}

struct camera {
    float hsize;
    float vsize;
    float fov;
    matrix4x4 transform;
    float half_width;
    float half_height;
    float pixel_size;
    void compute_pixel_size();
    camera();
    camera(int hsize, int vsize, float fov);
    camera(int hsize, int vsize, float fov, matrix4x4 transform);
    ray::ray ray_for_pixel(int x, int y);
    canvas render(world w);
};

void camera::compute_pixel_size() {
    float half_view = tan(this->fov / 2);
    float aspect_ratio = this->hsize / this->vsize;
    if (aspect_ratio >= 1) {
        this->half_width = half_view;
        this->half_height = half_view / aspect_ratio;
    } else {
        this->half_width = half_view * aspect_ratio;
        this->half_height = half_view;
    }
    this->pixel_size = (this->half_width * 2) / this->hsize;
}

camera::camera() {
    this->hsize = 160;
    this->vsize = 120;
    this->fov = M_PI / 2;
    this->transform = mat::identity;
    this->compute_pixel_size();
}

camera::camera(int hsize, int vsize, float fov) {
    this->hsize = hsize;
    this->vsize = vsize;
    this->fov = fov;
    this->transform = mat::identity;
    this->compute_pixel_size();
}

camera::camera(int hsize, int vsize, float fov, matrix4x4 transform) {
    this->hsize = hsize;
    this->vsize = vsize;
    this->fov = fov;
    this->transform = transform;
    this->compute_pixel_size();
}

ray::ray camera::ray_for_pixel(int px, int py) {
    float x_offset = (px + 0.5) * this->pixel_size;
    float y_offset = (py + 0.5) * this->pixel_size;
    float world_x = this->half_width - x_offset;
    float world_y = this->half_height - y_offset;
    matrix4x4 inv_transf = this->transform.inverse();
    vector v = vec::point3(world_x, world_y, -1);
    vector pixel = inv_transf * v;
    vector o = vec::point3(0, 0, 0);
    vector origin = inv_transf * o;
    vector direction = (pixel - origin).normalize();

    return ray::ray{origin, direction};
}

canvas camera::render(world w) {
    canvas c{int(this->hsize), int(this->vsize)};
    for (int y = 0; y < this->vsize; y++) {
        for (int x = 0; x < this->hsize; x++) {
            ray::ray r{this->ray_for_pixel(x, y)};
            color col = w.color_at(r);
            c.write(x, y, col);
        }
    }

    return c;
}

#endif