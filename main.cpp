#include "rtrace.h"
#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"


void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0,  10, 0), 10, make_shared<lambertian>(checker)));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void balls() {
    // Defining Materials
    auto checker         = make_shared<checker_texture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9)); 
    auto material_ground = make_shared<lambertian>(checker);
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);

    // Making World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2),  0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0),  0.5, material_right));
    world = hittable_list(make_shared<bvh_node>(world));

    //Creating Camera / Rendering
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov     = 90;
    cam.lookfrom = point3(0,0,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 10.0;
    cam.focus_dist    = 2;

    cam.render(world);
}

void quads() {
    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    // auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto babic = make_shared<image_texture>("textures/redwood_pack.jpg");
    auto babic_surface = make_shared<lambertian>(babic);

    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    //Making World
    hittable_list world;
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), babic_surface));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    world = hittable_list(make_shared<bvh_node>(world));


    //Camera / render
    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void world() {
    auto earth_texture = make_shared<image_texture>("textures/redwood_pack.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void simple_light() {
    hittable_list world;

    auto grass_mat = make_shared<lambertian>(color(0.486,0.988,0.0));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, grass_mat));

    auto ball_mat = make_shared<metal>(color(0.6, 0.6, 0.6), 0);
    world.add(make_shared<sphere>(point3(0,2,0), 2, ball_mat));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.043, 0.063, 0.149);

    cam.vfov = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat = point3(0,2,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    switch(5){
        case 1: balls(); break;
        case 2: quads(); break;
        case 3: checkered_spheres(); break;
        case 4: world(); break;
        case 5: simple_light(); break;
    }
    
}