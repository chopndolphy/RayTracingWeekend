#pragma once
#include "rt_math.h"
#include "vec3.h"

#include <vector>

struct pixel {
    int r, g, b;
};

class framebuffer {
    public:
        framebuffer(int height, int width) : height(height), width(width) {
            buf = (pixel*)malloc(height * width * sizeof(pixel));
        }

        ~framebuffer() {
            free(buf);
        }

        pixel* buf;
        int height;
        int width;
};
