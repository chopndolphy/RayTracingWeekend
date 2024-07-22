#include "Application.h"
#include "Raytracer.h"
#include "Camera.h"

void Application::Run() {
    Camera camera(1920, 16.0 / 9.0, math::point3(1.0, 1.0, 1.0));
    Raytracer raytracer(&camera);
    raytracer.RenderScenePPM();
}