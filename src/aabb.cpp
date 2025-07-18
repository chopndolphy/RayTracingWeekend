#include "aabb.h"
const aabb aabb::empty = aabb(rtm::empty_interval, rtm::empty_interval, rtm::empty_interval);
const aabb aabb::universe = aabb(rtm::universe_interval, rtm::universe_interval, rtm::universe_interval);
