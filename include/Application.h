#pragma once

class Application {
    public: 
        void Run();
    private:
        void old_scene();
        void bouncing_spheres();
        void checkered_spheres();
        void earth();
        void perlin_spheres();
        void quads();
        void simple_light();
        void cornell_box();
        void cornell_smoke();
        void book_two_final(int image_width, int samples_per_pixel, int max_depth);
};
