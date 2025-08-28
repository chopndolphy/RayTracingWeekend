#pragma once

#include "rt_math.h"
#include "hittable_list.h"
#include "vec3.h"

class Camera {
    public:
        Camera();
        double aspectRatio  = 1.0;
        int imageWidth      = 100;
        int samplesPerPixel = 10;
        int maxDepth        = 10;
        rtm::color background;

        double vfov = 90;
        rtm::point3 lookFrom = rtm::point3(0, 0, 0);
        rtm::point3 lookAt = rtm::point3(0, 0, -1);
        rtm::vec3 viewUp = rtm::vec3(0, 1, 0);

        double defocusAngle = 0;
        double focusDistance = 10;
        
        void RenderScenePPM(const hittable_list& world);
    
    private:
        int imageHeight;
        double pixelSamplesScale;
        rtm::point3 center = rtm::point3{0.0, 0.0, 0.0};
        rtm::vec3 pixel00Loc;
        rtm::vec3 pixelDeltaU;
        rtm::vec3 pixelDeltaV;
        rtm::vec3 u, v, w;
        rtm::vec3 defocusDiskU;
        rtm::vec3 defocusDiskV;

        void initialize();
        rtm::color rayColor(const rtm::ray& r, int depth, const hittable& world) const;
        void writeColor(std::ostream &out, const rtm::color &pixelColor) const;
        rtm::ray getRay(int i, int j) const;
        rtm::vec3 sampleSquare() const;
        rtm::point3 defocusDiskSample() const;
};
