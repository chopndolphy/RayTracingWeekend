#pragma once

#include "vec3.h"

class Camera {
    public:
        Camera(unsigned int imageWidth, double aspectRatio, math::point3 cameraCenter)
        : imageWidth(imageWidth), aspectRatio(aspectRatio), cameraCenter(cameraCenter) {}

        const unsigned int ImageWidth() const { return imageWidth; }
        const unsigned int ImageHeight() const { return int(imageWidth / aspectRatio); }
        const double AspectRatio() const { return aspectRatio; }
        const double FocalLength() const { return focalLength; }
        const double ViewportHeight() const { return viewportHeight; }
        const double ViewportWidth() const { return viewportHeight * double(imageWidth)/ImageHeight(); }
        const math::point3 CameraCenter() const { return cameraCenter; }
        const math::vec3 ViewportU() const { return math::vec3(ViewportWidth(), 0, 0); }
        const math::vec3 ViewportV() const { return math::vec3(0, -viewportHeight, 0); }
        const math::vec3 PixelDeltaU() const { return ViewportU() / imageWidth; }
        const math::vec3 PixelDeltaV() const { return ViewportV() / ImageHeight(); }
        const math::vec3 ViewportUpperLeft() const {
            return cameraCenter - math::vec3(0, 0, focalLength) - ViewportU() / 2 - ViewportV() / 2;
        }
        const math::vec3 Pixel00Loc() const { return ViewportUpperLeft() + 0.5 * (PixelDeltaU() + PixelDeltaV()); }
    
    private:
        unsigned int imageWidth;
        double aspectRatio;
        math::point3 cameraCenter;
        double focalLength = 1.0;
        double viewportHeight = 2.0;
};