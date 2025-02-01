#include "Camera.h"

#include "material.h"
#include "rt_math.h"

#include <chrono>

Camera::Camera() {
    initialize();
}
void Camera::RenderScenePPM(const hittable_list& world) {
    initialize();    

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    int seconds;

    for (int j = 0; j < imageHeight; j++) {
        seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count();
        std::clog << "\r[" <<  std::floor(seconds/60) << "m" << seconds % 60 << "s] "
        << "Scanlines completed: " << j << '/' << imageHeight << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            rtm::color pixelColor(0, 0, 0);
            for (int sample = 0; sample < samplesPerPixel; sample++) {
                rtm::ray r = getRay(i, j);
                pixelColor += rayColor(r, maxDepth, world);
            }
            writeColor(std::cout, pixelSamplesScale * pixelColor);
        }
    }
    seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count();
    std::clog << "\rRendered successfully in " << std::floor(seconds/60) << "m" << seconds % 60 << "s."
    << "                                               " << std::endl;
}
void Camera::initialize() {
    imageHeight = int(imageWidth/aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    pixelSamplesScale = 1.0/samplesPerPixel;

    center = lookFrom;

    double theta = rtm::degrees_to_radians(vfov);
    double h = std::tan(theta/2);
    double viewportHeight = 2*h*focusDistance;
    double viewportWidth = viewportHeight*(double(imageWidth)/imageHeight);

    w = rtm::normalize(lookFrom - lookAt);
    u = rtm::normalize(rtm::cross(viewUp, w));
    v = rtm::cross(w, u);

    rtm::vec3 viewportU = viewportWidth*u;
    rtm::vec3 viewportV = viewportHeight*-v;

    pixelDeltaU = viewportU/imageWidth;
    pixelDeltaV = viewportV/imageHeight;

    rtm::vec3 viewportUpperLeft = center - (focusDistance*w) - viewportU/2 - viewportV/2;
    pixel00Loc = viewportUpperLeft + 0.5*(pixelDeltaU + pixelDeltaV);

    double defocusRadius = focusDistance*std::tan(rtm::degrees_to_radians(defocusAngle/2));
    defocusDiskU = u*defocusRadius;
    defocusDiskV = v*defocusRadius;
}
rtm::color Camera::rayColor(const rtm::ray &r, int depth, const hittable& world) const {
    if (depth <= 0) {
        return rtm::color(0, 0, 0);
    }

    hit_record rec;

    if (world.hit(r, rtm::interval(0.001, rtm::infinity), rec)) {
        rtm::ray scattered;
        rtm::color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation*rayColor(scattered, depth - 1, world);
        }
        return rtm::color(0, 0, 0);
    }

    rtm::vec3 unitDirection = rtm::normalize(r.direction());
    double a = 0.5*(unitDirection.y() + 1.0);
    return (1.0 - a)*rtm::color(1.0, 1.0, 1.0) + a*rtm::color(0.5, 0.7, 1.0);
}
void Camera::writeColor(std::ostream& out, const rtm::color& pixelColor) const {
    double r = pixelColor.x();
    double g = pixelColor.y();
    double b = pixelColor.z();

    r = rtm::linear_to_gamma(r);
    g = rtm::linear_to_gamma(g);
    b = rtm::linear_to_gamma(b);

    static const rtm::interval intensity(0.000, 0.999);
    int rbyte = int(256*intensity.clamp(r));
    int gbyte = int(256*intensity.clamp(g));
    int bbyte = int(256*intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
rtm::ray Camera::getRay(int i, int j) const {
    rtm::vec3 offset = sampleSquare();
    rtm::point3 pixelSample = pixel00Loc
                            + ((i + offset.x())*pixelDeltaU)
                            + ((j + offset.y())*pixelDeltaV);
    rtm::point3 rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
    rtm::vec3 rayDirection = pixelSample - rayOrigin;
    double ray_time = rtm::random_double();

    return rtm::ray(rayOrigin, rayDirection, ray_time);
}
rtm::vec3 Camera::sampleSquare() const {
    return rtm::vec3(rtm::random_double() - 0.5, rtm::random_double() - 0.5, 0);
}
rtm::point3 Camera::defocusDiskSample() const {
    auto p = rtm::random_in_unit_disk();
    return center + (p[0]*defocusDiskU) + (p[1]*defocusDiskV);
}
