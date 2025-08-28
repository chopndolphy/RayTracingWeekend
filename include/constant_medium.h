#pragma once

#include "interval.h"
#include "ray.h"
#include "rt_math.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"
#include "vec3.h"

class constant_medium : public hittable {
    public:
        constant_medium(std::shared_ptr<hittable> boundary, double density, std::shared_ptr<texture> tex)
        : boundary(boundary), neg_inv_density(-1/density), phase_function(std::make_shared<isotropic>(tex)) {

        }

        constant_medium(std::shared_ptr<hittable> boundary, double density, const rtm::color &albedo)
        : boundary(boundary), neg_inv_density(-1/density), phase_function(std::make_shared<isotropic>(albedo)) {

        }

        bool hit(const rtm::ray &r, rtm::interval ray_t, hit_record &rec) const override {
            hit_record rec1, rec2;

            if (!boundary->hit(r, rtm::universe_interval, rec1)) {
                return false;
            }

            if (!boundary->hit(r, rtm::interval(rec1.t+0.0001, rtm::infinity), rec2)) {
                return false;
            }

            if (rec1.t < ray_t.min) {
                rec1.t = ray_t.min;
            }
            if (rec2.t > ray_t.max) {
                rec2.t = ray_t.max;
            }

            if (rec1.t >= rec2.t) {
                return false;
            }

            if (rec1.t < 0) {
                rec1.t = 0;
            }

            auto ray_length = r.direction().length();
            auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
            auto hit_distance = neg_inv_density * std::log(rtm::random_double());

            if (hit_distance > distance_inside_boundary) {
                return false;
            }

            rec.t = rec1.t + hit_distance / ray_length;
            rec.p = r.at(rec.t);

            rec.normal = rtm::vec3(1, 0, 0);
            rec.front_face = true;
            rec.mat = phase_function;

            return true;
        }

        aabb bounding_box() const override { return boundary->bounding_box(); }

    private:
        std::shared_ptr<hittable> boundary;
        double neg_inv_density;
        std::shared_ptr<material> phase_function;
};
