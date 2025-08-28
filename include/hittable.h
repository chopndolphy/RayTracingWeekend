#pragma once

#include "interval.h"
#include "ray.h"
#include "rt_math.h"
#include "aabb.h"
#include "vec3.h"

class material;

class hit_record {
    public:
        rtm::point3 p;
        rtm::vec3 normal;
        std::shared_ptr<material> mat;
        double t;
        double u;
        double v;
        bool front_face;

        void set_face_normal(const rtm::ray& r, const rtm::vec3& outward_normal) {
            //outward_normal assumed to have unit length
            front_face = rtm::dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};
class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const rtm::ray& r, rtm::interval rayT, hit_record& rec) const = 0;

        virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
    public:
        translate(std::shared_ptr<hittable> object, const rtm::vec3 &offset)
        : object(object), offset(offset) {
            bbox = object->bounding_box() + offset;
        }

        bool hit(const rtm::ray &r, rtm::interval ray_t, hit_record &rec) const override {
            rtm::ray offset_r(r.origin() - offset, r.direction(), r.time());

            if (!object->hit(offset_r, ray_t, rec)) {
                return false;
            }

            rec.p += offset;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        std::shared_ptr<hittable> object;
        rtm::vec3 offset;
        aabb bbox;
};

class rotate_y : public hittable {
    public:
        rotate_y(std::shared_ptr<hittable> object, double angle) : object(object) {
            auto radians = rtm::degrees_to_radians(angle);
            sin_theta = std::sin(radians);
            cos_theta = std::cos(radians);
            bbox = object->bounding_box();

            rtm::point3 min(rtm::infinity, rtm::infinity, rtm::infinity);
            rtm::point3 max(-rtm::infinity, -rtm::infinity, -rtm::infinity);

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                        auto newx = cos_theta * x + sin_theta * z;
                        auto newz = -sin_theta * x + cos_theta * z;

                        rtm::vec3 tester(newx, y, newz);

                        for (int c = 0; c < 3; c++) {
                            min[c] = std::fmin(min[c], tester[c]);
                            max[c] = std::fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = aabb(min, max);
        }

        bool hit(const rtm::ray &r, rtm::interval ray_t, hit_record &rec) const override {
            auto origin = rtm::point3(
                (cos_theta * r.origin().x()) - (sin_theta * r.origin().z()),
                r.origin().y(),
                (sin_theta * r.origin().x()) + (cos_theta * r.origin().z())
            );

            auto direction = rtm::vec3(
                (cos_theta * r.direction().x()) - (sin_theta * r.direction().z()),
                r.direction().y(),
                (sin_theta * r.direction().x()) + (cos_theta * r.direction().z())
            );

            rtm::ray rotated_r(origin, direction, r.time());

            if (!object->hit(rotated_r, ray_t, rec)) {
                return false;
            }

            rec.p = rtm::point3(
                (cos_theta * rec.p.x()) + (sin_theta * rec.p.z()),
                rec.p.y(),
                (-sin_theta * rec.p.x()) + (cos_theta * rec.p.z())
            );

            rec.normal = rtm::vec3(
                (cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()),
                rec.normal.y(),
                (-sin_theta * rec.normal.x()) + (cos_theta * rec.normal.z())
            );

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
    std::shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};
