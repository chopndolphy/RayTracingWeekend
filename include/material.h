#pragma once

#include "rt_math.h"
#include "hittable.h"

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(
            const rtm::ray& rIn, const hit_record& rec, rtm::color& attentuation, rtm::ray& scattered
        ) const = 0;
};
class lambertian : public material {
    public:
        lambertian(const rtm::color& albedo) : albedo(albedo) {}

        bool scatter(
            const rtm::ray& rIn, const hit_record& rec, rtm::color& attenuation, rtm::ray& scattered
        ) const override {
            rtm::vec3 scatterDirection = rec.normal + rtm::random_unit_vector();

            if (scatterDirection.near_zero()) {
                scatterDirection = rec.normal;
            }

            scattered = rtm::ray(rec.p, scatterDirection);
            attenuation = albedo;
            return true;
        }
    private:
        rtm::color albedo;
};
class metal : public material {
    public:
        metal(const rtm::color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}


        bool scatter(
            const rtm::ray& rIn, const hit_record& rec, rtm::color& attenuation, rtm::ray& scattered
        ) const override {
            rtm::vec3 reflected = rtm::reflect(rIn.direction(), rec.normal);
            reflected = rtm::normalize(reflected) + (fuzz * rtm::random_unit_vector());
            scattered = rtm::ray(rec.p, reflected);
            attenuation = albedo;
            return (rtm::dot(scattered.direction(), rec.normal) > 0);
        }
    private:
        rtm::color albedo;
        double fuzz;
};
class dielectric : public material {
    public:
        dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

        bool scatter(
            const rtm::ray& rIn, const hit_record& rec, rtm::color& attenuation, rtm::ray& scattered
        ) const override {
            attenuation = rtm::color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refractionIndex) : refractionIndex;

            rtm::vec3 unitDirection = rtm::normalize(rIn.direction());
            double cosTheta = std::fmin(rtm::dot(-unitDirection, rec.normal), 1.0);
            double sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);
            
            bool cannotRefract = ri*sinTheta > 1.0;
            rtm::vec3 direction;

            if (cannotRefract || rtm::reflectance(cosTheta, ri) > rtm::random_double()) {
                direction = rtm::reflect(unitDirection, rec.normal);
            } else {
                direction = rtm::refract(unitDirection, rec.normal, ri);
            }
            scattered = rtm::ray(rec.p, direction);
            return true;
        }
    private:
        double refractionIndex;
};