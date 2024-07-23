#pragma once
#include <cmath>
#include <limits>

namespace rtm {
    class interval {
        public:
            double min;
            double max;

            // default is empty
            interval() : min(+std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}

            interval(double min, double max) : min(min), max(max) {}

            double size() const {
                return max - min;
            }
            bool contains(double x) const {
                return min <= x && x <= max;
            }
            bool surrounds(double x) const {
                return min < x && x < max;
            }
            double clamp(double x) const {
                if (x < min) return min;
                if (x > max) return max;
                return x;
            }
            
    };
    static const interval empty_interval    = interval(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    static const interval universe_interval = interval(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());
}