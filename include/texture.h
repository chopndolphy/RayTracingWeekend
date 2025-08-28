#pragma once

#include "rt_math.h"
#include "rtw_stb_image.h"
#include "perlin.h"
#include "vec3.h"

#include <memory>

class texture {
    public:
        virtual ~texture() = default;

        virtual rtm::color value(double u, double v, const rtm::point3 &p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const rtm::color &albedo) : albedo(albedo) {}

        solid_color(double red, double green, double blue) : solid_color(rtm::color(red, green, blue)) {}

        rtm::color value(double u, double v, const rtm::point3 &p) const override {
            return albedo;
        }

    private:
        rtm::color albedo;
};

class checker_texture : public texture {
    public:
        checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
            : inv_scale(1.0 / scale), even(even), odd(odd) {}

        checker_texture(double scale, const rtm::color &c1, const rtm::color &c2)
            : checker_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

        rtm::color value(double u, double v, const rtm::point3 &p) const override {
            auto xInteger = int(std::floor(inv_scale * p.x()));
            auto yInteger = int(std::floor(inv_scale * p.y()));
            auto zInteger = int(std::floor(inv_scale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u,v, p);
        }

    private:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
};

class image_texture : public texture {
    public:
        image_texture(const char* filename) : image(filename) {}
        
        rtm::color value(double u, double v, const rtm::point3 &p) const override {
            if (image.height() <= 0) return rtm::color(0, 1, 1);

            u = rtm::interval(0, 1).clamp(u);
            v = 1.0 - rtm::interval(0, 1).clamp(v);

            auto i = int(u * image.width());
            auto j = int(v * image.height());
            auto pixel = image.pixel_data(i, j);

            auto color_scale = 1.0 / 255.0;
            return rtm::color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

    private:
        rtw_image image;
};

class noise_texture : public texture {
    public:
        noise_texture(double scale) : scale(scale) {}

        rtm::color value(double u, double v, const rtm::point3 &p) const override {
            //return rtm::color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
            //return rtm::color(1, 1, 1) * noise.turb(p, 7);
            return rtm::color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
        }
    private:
        perlin noise;
        double scale;
};
