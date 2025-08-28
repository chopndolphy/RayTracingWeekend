#include "Application.h"
#include "Camera.h"
#include "hittable_list.h"
#include "rt_math.h"
#include "sphere.h"
#include "material.h"
#include "bvh.h"
#include "texture.h"
#include "vec3.h"
#include "quad.h"
#include "constant_medium.h"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

void Application::Run() {
    switch (9) {
        case 0:
            old_scene();
            break;
        case 1:
            bouncing_spheres();
            break;
        case 2:
            checkered_spheres();
            break;
        case 3:
            earth();
            break;
        case 4:
            perlin_spheres();
            break;
        case 5:
            quads();
            break;
        case 6:
            simple_light();
            break;
        case 7:
            cornell_box();
            break;
        case 8:
            cornell_smoke();
            break;
        case 9:
            book_two_final(800, 10000, 40);
            break;
        default:
            book_two_final(400, 250, 4);
            break;
    }
}

void Application::old_scene() {
    hittable_list world;

    std::unordered_map<std::string, std::shared_ptr<material>> matList;
    matList.try_emplace("ground", std::make_shared<lambertian>(rtm::color(0.259, 0.412, 0.212)));
    matList.try_emplace("center", std::make_shared<lambertian>(rtm::color(0.588, 0.0, 0.039)));
    matList.try_emplace("left", std::make_shared<dielectric>(1.50));
    matList.try_emplace("bubble", std::make_shared<dielectric>(1.0/1.50));
    matList.try_emplace("right", std::make_shared<metal>(rtm::color(0.8, 0.6, 0.2), 1.0));

    world.add(std::make_shared<sphere>(rtm::point3( 0.0, -100.5, -1.0), 100.0, matList.at("ground")));
    world.add(std::make_shared<sphere>(rtm::point3( 0.0,    0.0, -1.2),   0.5, matList.at("center")));
    world.add(std::make_shared<sphere>(rtm::point3(-1.0,    0.0, -1.0),   0.5, matList.at("left")));
    world.add(std::make_shared<sphere>(rtm::point3(-1.0,    0.0, -1.0),   0.4, matList.at("bubble")));
    world.add(std::make_shared<sphere>(rtm::point3( 1.0,    0.0, -1.0),   0.5, matList.at("right")));

    Camera cam;

    cam.aspectRatio     = 16.0/9.0;
    cam.imageWidth      = 960;
    cam.samplesPerPixel = 100;
    cam.maxDepth        = 50;

    cam.vfov     = 90;
    cam.lookFrom = rtm::point3(-2, 2, 1);
    cam.lookAt   = rtm::point3(0, 0, -1);
    cam.viewUp   = rtm::vec3(0, 1, 0);

    cam.defocusAngle  = 10.0;
    cam.focusDistance = 3.4;

    cam.RenderScenePPM(world);
}

void Application::bouncing_spheres() {

    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, rtm::color(0.2, 0.3, 0.1), rtm::color(0.9, 0.9, 0.9));
    world.add(std::make_shared<sphere>(rtm::point3(0, -1000, 0), 1000, std::make_shared<lambertian>(checker)));

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
                    world.add(std::make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (chooseMat < 0.8) {
                    rtm::color albedo = rtm::random_vec3(0.5, 1);
                    double fuzz = rtm::random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (chooseMat < 0.9) {
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                    
                    sphere_material = std::make_shared<dielectric>(1.0/1.50);
                    world.add(std::make_shared<sphere>(center, 0.16, sphere_material));
                }
            }
        }
    }
    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(rtm::point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(rtm::color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(rtm::point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(rtm::color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(rtm::point3(4, 1, 0), 1.0, material3));

    world = hittable_list(std::make_shared<bvh_node>(world));

    Camera cam;

    cam.aspectRatio     = 16.0/9.0;
    cam.imageWidth      = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth        = 50;
    cam.background      = rtm::color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookFrom = rtm::point3(13, 2, 3);
    cam.lookAt   = rtm::point3(0, 0, 0);
    cam.viewUp   = rtm::vec3(0, 1, 0);

    cam.defocusAngle  = 0.6;
    cam.focusDistance = 10.0;

    cam.RenderScenePPM(world);
}

void Application::checkered_spheres() {
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(0.32, rtm::color(0.2, 0.3, 0.1), rtm::color(0.9, 0.9, 0.9));

    world.add(std::make_shared<sphere>(rtm::point3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(rtm::point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = rtm::color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = rtm::point3(13, 2 ,3);
    cam.lookAt = rtm::point3(0, 0, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.RenderScenePPM(world);
}

void Application::earth() {
    auto earth_texture = std::make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = std::make_shared<sphere>(rtm::point3(0, 0, 0), 2, earth_surface);

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = rtm::color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = rtm::point3(0, 0, 12);
    cam.lookAt = rtm::point3(0, 0, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.RenderScenePPM(hittable_list(globe));
}

void Application::perlin_spheres() {
    hittable_list world;

    auto pertext = std::make_shared<noise_texture>(4);
    world.add(std::make_shared<sphere>(rtm::point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(rtm::point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = rtm::color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = rtm::point3(13, 2, 3);
    cam.lookAt = rtm::point3(0, 0, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.RenderScenePPM(world);
}

void Application::quads() {
    hittable_list world;

    auto left_red     = std::make_shared<lambertian>(rtm::color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<lambertian>(rtm::color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<lambertian>(rtm::color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<lambertian>(rtm::color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<lambertian>(rtm::color(0.2, 0.8, 0.8));

    world.add(std::make_shared<quad>(rtm::point3(-3, -2, 5), rtm::vec3(0, 0, -4), rtm::vec3(0, 4,  0), left_red));
    world.add(std::make_shared<quad>(rtm::point3(-2, -2, 0), rtm::vec3(4, 0,  0), rtm::vec3(0, 4,  0), back_green));
    world.add(std::make_shared<quad>(rtm::point3( 3, -2, 1), rtm::vec3(0, 0,  4), rtm::vec3(0, 4,  0), right_blue));
    world.add(std::make_shared<quad>(rtm::point3(-2,  3, 1), rtm::vec3(4, 0,  0), rtm::vec3(0, 0,  4), upper_orange));
    world.add(std::make_shared<quad>(rtm::point3(-2, -3, 5), rtm::vec3(4, 0,  0), rtm::vec3(0, 0, -4), lower_teal));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = rtm::color(0.70, 0.80, 1.00);

    cam.vfov = 80;
    cam.lookFrom = rtm::point3(0, 0, 9);
    cam.lookAt = rtm::point3(0, 0, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.RenderScenePPM(world);
}

void Application::simple_light() {
    hittable_list world;

    auto pertext = std::make_shared<noise_texture>(4);
    world.add(std::make_shared<sphere>(rtm::point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(rtm::point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    auto difflight = std::make_shared<diffuse_light>(rtm::color(4, 4, 4));
    world.add(std::make_shared<sphere>(rtm::point3(0, 7, 0), 2, difflight));
    world.add(std::make_shared<quad>(rtm::point3(3, 1, -2), rtm::vec3(2, 0, 0), rtm::vec3(0, 2, 0), difflight));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;
    cam.background = rtm::color(0, 0, 0);

    cam.vfov = 20;
    cam.lookFrom = rtm::point3(26, 3, 6);
    cam.lookAt = rtm::point3(0, 2, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;
    
    cam.RenderScenePPM(world);
}

void Application::cornell_box() {
    hittable_list world;

    auto red     = std::make_shared<lambertian>(rtm::color(0.65, 0.05, 0.05));
    auto white   = std::make_shared<lambertian>(rtm::color(0.73, 0.73, 0.73));
    auto green   = std::make_shared<lambertian>(rtm::color(0.12, 0.45, 0.15));
    auto light   = std::make_shared<diffuse_light>(rtm::color(15, 15, 15));

    world.add(std::make_shared<quad>(rtm::point3(555, 0, 0), rtm::vec3(0, 555, 0), rtm::vec3(0, 0, 555), green));
    world.add(std::make_shared<quad>(rtm::point3(0, 0, 0), rtm::vec3(0, 555, 0), rtm::vec3(0, 0, 555), red));
    world.add(std::make_shared<quad>(rtm::point3(343, 554, 332), rtm::vec3(-130, 0, 0), rtm::vec3(0, 0, -105), light));
    world.add(std::make_shared<quad>(rtm::point3(0, 0, 0), rtm::vec3(555, 0, 0), rtm::vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(rtm::point3(555, 555, 555), rtm::vec3(-555, 0, 0), rtm::vec3(0, 0, -555), white));
    world.add(std::make_shared<quad>(rtm::point3(0, 0, 555), rtm::vec3(555, 0, 0), rtm::vec3(0, 555, 0), white));

    std::shared_ptr<hittable> box1 = box(rtm::point3(0, 0, 0), rtm::point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, rtm::vec3(265, 0, 295));
    world.add(box1);

    std::shared_ptr<hittable> box2 = box(rtm::point3(0, 0, 0), rtm::point3(165, 165, 165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, rtm::vec3(130, 0, 65));
    world.add(box2);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = rtm::color(0, 0, 0);

    cam.vfov = 40;
    cam.lookFrom = rtm::point3(278, 278, -800);
    cam.lookAt = rtm::point3(278, 278, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;
    
    cam.RenderScenePPM(world);
}

void Application::cornell_smoke() {
    hittable_list world;

    auto red     = std::make_shared<lambertian>(rtm::color(0.65, 0.05, 0.05));
    auto white   = std::make_shared<lambertian>(rtm::color(0.73, 0.73, 0.73));
    auto green   = std::make_shared<lambertian>(rtm::color(0.12, 0.45, 0.15));
    auto light   = std::make_shared<diffuse_light>(rtm::color(7, 7, 7));

    world.add(std::make_shared<quad>(rtm::point3(555, 0, 0), rtm::vec3(0, 555, 0), rtm::vec3(0, 0, 555), green));
    world.add(std::make_shared<quad>(rtm::point3(0, 0, 0), rtm::vec3(0, 555, 0), rtm::vec3(0, 0, 555), red));
    world.add(std::make_shared<quad>(rtm::point3(113, 554, 127), rtm::vec3(330, 0, 0), rtm::vec3(0, 0, 305), light));
    world.add(std::make_shared<quad>(rtm::point3(0, 555, 0), rtm::vec3(555, 0, 0), rtm::vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(rtm::point3(0, 0, 0), rtm::vec3(555, 0, 0), rtm::vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(rtm::point3(0, 0, 555), rtm::vec3(555, 0, 0), rtm::vec3(0, 555, 0), white));

    std::shared_ptr<hittable> box1 = box(rtm::point3(0, 0, 0), rtm::point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, rtm::vec3(265, 0, 295));
    world.add(std::make_shared<constant_medium>(box1, 0.01, rtm::color(0, 0, 0)));

    std::shared_ptr<hittable> box2 = box(rtm::point3(0, 0, 0), rtm::point3(165, 165, 165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, rtm::vec3(130, 0, 65));
    world.add(std::make_shared<constant_medium>(box2, 0.01, rtm::color(1, 1, 1)));

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 200;
    cam.maxDepth = 50;
    cam.background = rtm::color(0, 0, 0);

    cam.vfov = 40;
    cam.lookFrom = rtm::point3(278, 278, -800);
    cam.lookAt = rtm::point3(278, 278, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;
    
    cam.RenderScenePPM(world);
}

void Application::book_two_final(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list boxes1;
    auto ground = std::make_shared<lambertian>(rtm::color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = rtm::random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(rtm::point3(x0, y0, z0), rtm::point3(x1, y1, z1), ground));
        }
    }

    hittable_list world;

    world.add(std::make_shared<bvh_node>(boxes1));

    auto light = std::make_shared<diffuse_light>(rtm::color(7, 7, 7));
    world.add(std::make_shared<quad>(rtm::point3(123, 554, 147), rtm::vec3(300, 0, 0), rtm::vec3(0, 0, 265), light));

    auto center1 = rtm::point3(400, 400, 200);
    auto center2 = center1 + rtm::vec3(30, 0, 0);
    auto sphere_material = std::make_shared<lambertian>(rtm::color(0.7, 0.3, 0.1));
    world.add(std::make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(std::make_shared<sphere>(rtm::point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
    world.add(std::make_shared<sphere>(
        rtm::point3(0, 150, 145), 50, std::make_shared<metal>(rtm::color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(rtm::point3(360, 150, 145), 70, std::make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<constant_medium>(boundary, 0.2, rtm::color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<sphere>(rtm::point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
    world.add(std::make_shared<constant_medium>(boundary, 0.0001, rtm::color(1, 1, 1)));

    auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("earthmap.jpg"));
    world.add(std::make_shared<sphere>(rtm::point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<noise_texture>(0.2);
    world.add(std::make_shared<sphere>(rtm::point3(220, 280, 300), 80, std::make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = std::make_shared<lambertian>(rtm::color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(rtm::random_vec3(0, 165), 10, white));
    }

    world.add(std::make_shared<translate>(
            std::make_shared<rotate_y>(
                std::make_shared<bvh_node>(boxes2), 15),
            rtm::vec3(-100, 270, 395))
    );

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = image_width;
    cam.samplesPerPixel = samples_per_pixel;
    cam.maxDepth = max_depth;
    cam.background = rtm::color(0, 0, 0);

    cam.vfov = 40;
    cam.lookFrom = rtm::point3(478, 278, -600);
    cam.lookAt = rtm::point3(278, 278, 0);
    cam.viewUp = rtm::vec3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.RenderScenePPM(world);
}
