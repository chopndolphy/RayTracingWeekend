#include "aabb.h"

const aabb aabb::empty = aabb(rtm::empty_interval, rtm::empty_interval, rtm::empty_interval);
const aabb aabb::universe = aabb(rtm::universe_interval, rtm::universe_interval, rtm::universe_interval);

aabb operator+(const aabb &bbox, const rtm::vec3 &offset) {
    return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator+(const rtm::vec3 &offset, const aabb &bbox) {
    return bbox + offset;
}

// JANKYYYYYY
rtm::interval operator+(const rtm::interval &ival, double displacement) {
    return rtm::interval(ival.min + displacement, ival.max + displacement);
}

rtm::interval operator+(double displacement, const rtm::interval &ival) {
    return ival + displacement;
}
// JANKYYYYYYY
