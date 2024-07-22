#pragma once

#include "Camera.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"

class Raytracer {
    public:
        Raytracer(Camera* camera) : camera(camera) {}
        void RenderScenePPM();
    private:
        Camera* camera;
        math::color rayColor(const math::ray& r);
};