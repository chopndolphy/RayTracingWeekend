#pragma once

#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include <memory>

class quad : public hittable {
    public:
        quad(const rtm::point3 &Q, const rtm::vec3 &u, const rtm::vec3 &v, std::shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
            auto n = rtm::cross(u, v);
            normal = rtm::normalize(n);
            D = rtm::dot(normal, Q);
            w = n / rtm::dot(n, n);

            set_bounding_box();
        }

        virtual void set_bounding_box() {
            auto bbox_diagonal1 = aabb(Q, Q + u + v);
            auto bbox_diagonal2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override { return bbox; }

        bool hit(const rtm::ray &r, rtm::interval ray_t, hit_record &rec) const override {
            auto denom = rtm::dot(normal, r.direction());

            if (std::fabs(denom) < 1e-8) {
                return false;
            }

            auto t = (D - rtm::dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t)) {
                return false;
            }

            auto intersection = r.at(t);
            rtm::vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = rtm::dot(w, rtm::cross(planar_hitpt_vector, v));
            auto beta = rtm::dot(w, rtm::cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec)) {
                return false;
            }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

        virtual bool is_interior(double a, double b, hit_record &rec) const {
            rtm::interval unit_interval = rtm::interval(0, 1);

            if (!unit_interval.contains(a) || !unit_interval.contains(b)) { 
                return false;
            }

            rec.u = a;
            rec.v = b;
            return true;
        }

    private:
        rtm::point3 Q;
        rtm::vec3 u;
        rtm::vec3 v;
        rtm::vec3 w;
        std::shared_ptr<material> mat;
        aabb bbox;
        rtm::vec3 normal;
        double D;
};

inline std::shared_ptr<hittable_list> box(const rtm::point3 &a, const rtm::point3 &b, std::shared_ptr<material> mat) {
    auto sides = std::make_shared<hittable_list>();

    auto min = rtm::point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
    auto max = rtm::point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

    auto dx = rtm::vec3(max.x() - min.x(), 0, 0);
    auto dy = rtm::vec3(0, max.y() - min.y(), 0);
    auto dz = rtm::vec3(0, 0, max.z() - min.z());

    sides->add(std::make_shared<quad>(rtm::point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(std::make_shared<quad>(rtm::point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(std::make_shared<quad>(rtm::point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(std::make_shared<quad>(rtm::point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(std::make_shared<quad>(rtm::point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(std::make_shared<quad>(rtm::point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}
