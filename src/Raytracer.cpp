#include "Raytracer.h"

#include "Camera.h"
#include "ray.h"
#include "color.h"
#include "vec3.h"

#include <iostream>

void Raytracer::RenderScenePPM() {
    unsigned int imageHeight = camera->ImageHeight();
    unsigned int imageWidth = camera->ImageWidth();
    math::point3 pixel00 = camera->Pixel00Loc();
    math::vec3 pixelDeltaU = camera->PixelDeltaU();
    math::vec3 pixelDeltaV = camera->PixelDeltaV();
    math::point3 cameraCenter = camera->CameraCenter();

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            math::point3 pixelCenter = pixel00 + (i * pixelDeltaU) + (j * pixelDeltaV);
            math::vec3 rayDirection = pixelCenter - cameraCenter;
            math::ray r(cameraCenter, rayDirection);

            math::color pixelColor = rayColor(r);
            write_color(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
}
math::color Raytracer::rayColor(const math::ray &r) {
    math::vec3 unitDirection = unit_vector(r.direction());
    double a = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - a) * math::color(1.0, 1.0, 1.0) + a * math::color(0.5, 0.7, 1.0);
}
