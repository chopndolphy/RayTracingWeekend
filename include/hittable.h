#pragma once

#include "rt_math.h"
#include "aabb.h"

class material;

class hit_record {
    public:
        rtm::point3 p;
        rtm::vec3 normal;
        std::shared_ptr<material> mat;
        double t;
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
