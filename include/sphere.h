#pragma once

#include "rt_math.h"
#include "hittable.h"

class sphere : public hittable {
    public:
        // Stationary Sphere
        sphere(const rtm::point3& static_center, double radius, std::shared_ptr<material> mat)
            : center(static_center, rtm::vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}

        // Moving Sphere
        sphere(const rtm::point3& center1, const rtm::point3& center2, double radius, std::shared_ptr<material> mat)
            : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const rtm::ray &r, rtm::interval rayT, hit_record &rec) const override {
        rtm::point3 current_center = center.at(r.time());
        rtm::vec3 oc = current_center - r.origin();
        double a = r.direction().length_squared();
        double h = rtm::dot(r.direction(), oc);
        double c = oc.length_squared() - radius*radius;

        double discriminant = h*h - a*c;
        if (discriminant < 0) {
            return false;
        }

        double sqrtd = std::sqrt(discriminant);

        double root = (h - sqrtd)/a;
        if (!rayT.surrounds(root)) {
            root = (h + sqrtd)/a;
            if (!rayT.surrounds(root)) {
                return false;
            }
        } 
        rec.t = root;
        rec.p = r.at(rec.t);
        rtm::vec3 outward_normal = (rec.p - current_center)/radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }
    private:
        rtm::ray center;
        double radius;
        std::shared_ptr<material> mat;
};
