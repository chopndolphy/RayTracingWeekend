#pragma once

#include "vec3.h"
#include "ray.h"

#include <iostream>

namespace math {
    using color = vec3;


    inline void write_color(std::ostream& out, const math::color& pixel_color) {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        int rbyte = int(255.999 * r);
        int gbyte = int(255.999 * g);
        int bbyte = int(255.999 * b);

        out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    }
}