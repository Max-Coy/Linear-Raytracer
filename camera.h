#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera{
    public:
        double aspect_ratio     = 1.0; //image width/height
        int image_width         = 100; //pixel width of rendered image
        int samples_per_pixel   = 10;  //random samples per pixel
        int max_depth           = 10;  //maximum number of ray bounces in scene
        color background;

        double vfov     = 90; 
        point3 lookfrom = point3(0,0,0);
        point3 lookat   = point3(0,0,-1);
        vec3   vup      = vec3(0,1,0);

        double defocus_angle = 0;
        double focus_dist = 10;

        void render(const hittable& world){
            initialize();
            std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
            
            for(int j = 0; j < image_height; j++){
                std::clog << "\rScanlines remaining: " << (image_height - j) << " " << std::flush;
                for(int i = 0; i < image_width; i++){
                    color pixel_color(0,0,0);
                    for(int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i,j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);

                }
            }
            std::clog << "\rDone.                     \n";

        }
    private:
        int    image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel_00;
        vec3   pixel_du;
        vec3   pixel_dv;
        vec3   u, v, w;
        vec3   defocus_disk_u;
        vec3   defocus_disk_v;

        void initialize(){
            image_height = int(image_width/aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            pixel_samples_scale = 1.0 / samples_per_pixel;
            
            center = lookfrom;

            // Setting Camera
            auto theta = degree_to_radian(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate basis vectors for camera orientation
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w,u);

            // Camera Basis Vectors
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;
            
            pixel_du = viewport_u / image_width;
            pixel_dv = viewport_v / image_height;

            // Getting Upper left pixel
            auto viewport_origin = center - (focus_dist*w) - viewport_u/2 - viewport_v/2;
            pixel_00 = viewport_origin + 0.5 * (pixel_du + pixel_dv);

            // Calculate the camera defocus disk basis vectors
            auto defocus_radius = focus_dist * std::tan(degree_to_radian(defocus_angle/2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        ray get_ray(int i, int j) const{
            // Construct Camera Ray from defocus disk towards random point around pixel i, j
            auto offset = sample_square();
            auto pixel_sample = pixel_00
                              + ((i + offset.x()) * pixel_du)
                              + ((j + offset.y()) * pixel_dv);
            
            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_dir = pixel_sample - ray_origin;

            return ray(ray_origin, ray_dir);
        }


        vec3 sample_square() const{
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const{
            // Returns a random point in the defocus disk
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world) const{
            // If we exceed ray bounce limit, no more light is gathered
            if(depth <= 0)
                return color(0,0,0);

            hit_record rec;
            if(!world.hit(r, interval(0.001,infinity), rec))
                return background; // Ray didn't hit anything -> return bg color

            ray scattered;
            color attenuation;
            color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

            if(!rec.mat->scatter(r, rec, attenuation, scattered))
                return color_from_emission;

            color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

            return color_from_emission + color_from_scatter;
        }
};
#endif