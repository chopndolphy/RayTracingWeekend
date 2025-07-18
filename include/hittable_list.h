#pragma once

#include "aabb.h"
#include "hittable.h"
#include "rt_math.h"

#include <vector>

class hittable_list : public hittable {
    public:
        std::vector<std::shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }

        void add(std::shared_ptr<hittable> object) {
            objects.push_back(object);
            bbox = aabb(bbox, object->bounding_box());
        }

        bool hit(const rtm::ray& r, rtm::interval rayT, hit_record& rec) const override {
            hit_record tempRec;
            bool hitAnything = false;
            double closestSoFar = rayT.max;

            for (const auto& object : objects) {
                if (object->hit(r, rtm::interval(rayT.min, closestSoFar), tempRec)) {
                    hitAnything = true;
                    closestSoFar = tempRec.t;
                    rec = tempRec;
                }
            }

            return hitAnything;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        aabb bbox;
};
