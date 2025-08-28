#pragma once

#include "interval.h"
#include "rt_math.h"
#include "vec3.h"

class aabb {
    public:
        rtm::interval x;
        rtm::interval y;
        rtm::interval z;

        aabb() {}

        aabb(const rtm::interval &x, const rtm::interval &y, const rtm::interval &z)
        : x(x), y(y), z(z) {
            pad_to_minimums();
        }

        aabb(const rtm::point3 &a, const rtm::point3 &b) {
            x = (a[0] <= b[0]) ? rtm::interval(a[0], b[0]) : rtm::interval(b[0], a[0]);
            y = (a[1] <= b[1]) ? rtm::interval(a[1], b[1]) : rtm::interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? rtm::interval(a[2], b[2]) : rtm::interval(b[2], a[2]);

            pad_to_minimums();
        }

        aabb(const aabb &box0, const aabb &box1) {
            x = rtm::interval(box0.x, box1.x);
            y = rtm::interval(box0.y, box1.y);
            z = rtm::interval(box0.z, box1.z);
        }

        const rtm::interval& axis_interval(int n) const {
            if (n == 1) return y;
            if (n == 2) return z;
            return x;
        }

        bool hit(const rtm::ray &r, rtm::interval ray_t) const {
            const rtm::point3& ray_orig = r.origin();
            const rtm::vec3& ray_dir = r.direction();

            for (int axis = 0; axis < 3; axis++) {
                const rtm::interval& ax = axis_interval(axis);
                const double adinv = 1.0 / ray_dir[axis];

                auto t0 = (ax.min - ray_orig[axis]) * adinv;
                auto t1 = (ax.max - ray_orig[axis]) * adinv;

                if (t0 < t1) {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                } else {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }

                if (ray_t.max <= ray_t.min) {
                    return false;
                }
            }
            return true;
        }

        int longest_axis() const {
            if (x.size() > y.size()) {
                return x.size() > z.size() ? 0 : 2;
            } else {
                return y.size() > z.size() ? 1 : 2;
            }
        }

        static const aabb empty;
        static const aabb universe;

    private:

        void pad_to_minimums() {
            double delta = 0.0001;
            if (x.size() < delta) x = x.expand(delta);
            if (y.size() < delta) y = y.expand(delta);
            if (z.size() < delta) z = z.expand(delta);
        }
};

aabb operator+(const aabb &bbox, const rtm::vec3 &offset);
aabb operator+(const rtm::vec3 &offset, const aabb &bbox);
