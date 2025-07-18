#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include "ray.h"
#include "interval.h"
#include "vec3.h"

namespace rtm {
    constexpr double infinity = std::numeric_limits<double>::infinity();
    constexpr double pi = 3.141592653589793238462643383;

    inline double degrees_to_radians(double degrees) {
        return degrees * pi/180.0;
    }
    inline double random_double() {
        // 0 <= value < 1
        return rand()/(RAND_MAX + 1.0);
    }
    inline double random_double(double min, double max) {
        return min + (max-min)*random_double();
    }
    inline int random_int(int min, int max) {
        return int(random_double(min, max+1));
    }
    inline vec3 random_vec3() {
        return vec3(random_double(), random_double(), random_double());
    }
    inline vec3 random_vec3(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
    inline vec3 random_in_unit_sphere() {
        while (true) {
            vec3 p = random_vec3(-1, 1);
            if (p.length_squared() < 1)
                return p;
        }
    }
    inline vec3 random_in_unit_disk() {
        while (true) {
            vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
            if (p.length_squared() < 1)
                return p;
        }
    }
    inline vec3 random_unit_vector() {
        return normalize(random_in_unit_sphere());
    }
    inline vec3 random_on_hemisphere(const vec3& normal) {
        vec3 onUnitSphere = random_unit_vector();
        if (dot(onUnitSphere, normal) > 0.0) {
            return onUnitSphere;
        } else {
            return -onUnitSphere;
        }
    }
    inline vec3 reflect(const vec3& v, const vec3& n) {
        return v - 2*dot(v, n)*n;
    }
    inline vec3 refract(const vec3& uv, const vec3& n, double etaIOverEtaT) {
        double cosTheta = std::fmin(dot(-uv, n), 1.0);
        vec3 rOutPerp = etaIOverEtaT*(uv + cosTheta*n);
        vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.length_squared()))*n;
        return rOutPerp + rOutParallel;
    }
    inline double reflectance(double cosine, double refractionIndex) {
        // Schlick approximation
        double r0 = (1 - refractionIndex)/(1 + refractionIndex);
        r0 = r0*r0;
        return r0 + (1 - r0)*std::pow((1 - cosine), 5);
    }
    inline double linear_to_gamma(double linear_component) {
        if (linear_component > 0) {
            return std::sqrt(linear_component);
        }
        return 0;
    }
}
