#include "Application.h"
#include "Camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "bvh.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

void Application::Run() {
    // hittable_list world;

    // std::unordered_map<std::string, std::shared_ptr<material>> matList;
    // matList.try_emplace("ground", std::make_shared<lambertian>(rtm::color(0.259, 0.412, 0.212)));
    // matList.try_emplace("center", std::make_shared<lambertian>(rtm::color(0.588, 0.0, 0.039)));
    // matList.try_emplace("left", std::make_shared<dielectric>(1.50));
    // matList.try_emplace("bubble", std::make_shared<dielectric>(1.0/1.50));
    // matList.try_emplace("right", std::make_shared<metal>(rtm::color(0.8, 0.6, 0.2), 1.0));

    // world.add(std::make_shared<sphere>(rtm::point3( 0.0, -100.5, -1.0), 100.0, matList.at("ground")));
    // world.add(std::make_shared<sphere>(rtm::point3( 0.0,    0.0, -1.2),   0.5, matList.at("center")));
    // world.add(std::make_shared<sphere>(rtm::point3(-1.0,    0.0, -1.0),   0.5, matList.at("left")));
    // world.add(std::make_shared<sphere>(rtm::point3(-1.0,    0.0, -1.0),   0.4, matList.at("bubble")));
    // world.add(std::make_shared<sphere>(rtm::point3( 1.0,    0.0, -1.0),   0.5, matList.at("right")));

    // Camera cam;

    // cam.aspectRatio     = 16.0/9.0;
    // cam.imageWidth      = 960;
    // cam.samplesPerPixel = 100;
    // cam.maxDepth        = 50;

    // cam.vfov     = 90;
    // cam.lookFrom = rtm::point3(-2, 2, 1);
    // cam.lookAt   = rtm::point3(0, 0, -1);
    // cam.viewUp   = rtm::vec3(0, 1, 0);

    // cam.defocusAngle  = 10.0;
    // cam.focusDistance = 3.4;

    // cam.RenderScenePPM(world);

    hittable_list bookWorld;

    auto ground_material = std::make_shared<lambertian>(rtm::color(0.5, 0.5, 0.5));
    bookWorld.add(std::make_shared<sphere>(rtm::point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double chooseMat = rtm::random_double();
            rtm::point3 center(a + 0.9*rtm::random_double(), 0.2, b + 0.9*rtm::random_double());

            if ((center - rtm::point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (chooseMat < 0.6) {
                    rtm::color albedo = rtm::random_vec3()*rtm::random_vec3();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    rtm::point3 center2 = center + rtm::vec3(0, rtm::random_double(0, 0.5), 0);
                    bookWorld.add(std::make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (chooseMat < 0.8) {
                    rtm::color albedo = rtm::random_vec3(0.5, 1);
                    double fuzz = rtm::random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    bookWorld.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (chooseMat < 0.9) {
                    sphere_material = std::make_shared<dielectric>(1.5);
                    bookWorld.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = std::make_shared<dielectric>(1.5);
                    bookWorld.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                    
                    sphere_material = std::make_shared<dielectric>(1.0/1.50);
                    bookWorld.add(std::make_shared<sphere>(center, 0.16, sphere_material));
                }
            }
        }
    }
    auto material1 = std::make_shared<dielectric>(1.5);
    bookWorld.add(std::make_shared<sphere>(rtm::point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(rtm::color(0.4, 0.2, 0.1));
    bookWorld.add(std::make_shared<sphere>(rtm::point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(rtm::color(0.7, 0.6, 0.5), 0.0);
    bookWorld.add(std::make_shared<sphere>(rtm::point3(4, 1, 0), 1.0, material3));

    bookWorld = hittable_list(std::make_shared<bvh_node>(bookWorld));

    Camera bookCam;

    bookCam.aspectRatio     = 16.0/9.0;
    bookCam.imageWidth      = 400;
    bookCam.samplesPerPixel = 100;
    bookCam.maxDepth        = 50;

    bookCam.vfov     = 20;
    bookCam.lookFrom = rtm::point3(13, 2, 3);
    bookCam.lookAt   = rtm::point3(0, 0, 0);
    bookCam.viewUp   = rtm::vec3(0, 1, 0);

    bookCam.defocusAngle  = 0.6;
    bookCam.focusDistance = 10.0;

    bookCam.RenderScenePPM(bookWorld);
}
