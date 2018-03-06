#include "../geometry/scene.h"

#ifndef SHADER_H
#define SHADER_H

// Used to model different types of surfaces, e.g. matte, glossy, etc.
class Shader {
public:
    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light, const int num_shadow_rays) const = 0;

    // return: the proportion by which light is let through the
    //         material. E.g. a value of 1 is totally transparent, and a value
    //         of 0 is totally opaque.
    virtual float transparency() const {
        return 0.0f;
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account. If no light makes it from the light to the
    //         position, the color of the position is black.
    virtual vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light, const int num_shadow_rays) const {
        // How much the light ray penetrates to the light source.
        // This is higher if it travels through transparent objects.
        float acc_transparency = this->mean_random_transparency(position, prim, scene, light, num_shadow_rays);

        const vec3 col = this->color(position, prim, incoming, scene, light, num_shadow_rays);
        return acc_transparency * col;
    }

private:
    // return: the mean transparency from the intersection position to the
    //         random points in the sphere of the light source.
    float mean_random_transparency(vec4 pos, const Primitive *prim, const Scene &scene, const PointLight &light, const int num_shadow_rays) const {
        vector<Ray> shadow_rays = light.random_shadow_rays_from(pos, num_shadow_rays);

        // The addition of all transparencies of the different shadow rays.
        float acc_add_transparency = 0.0f;

        for (Ray shadow_ray: shadow_rays) {
            acc_add_transparency += this->shadow_ray_transparency(pos, prim, scene, shadow_ray);
        }

        // Calculate the mean by dividing by the number of rays.
        return acc_add_transparency / shadow_rays.size();
    }

    // return: the how much a light ray penetrates from an intersection to
    //         the light source.
    float shadow_ray_transparency(vec4 pos, const Primitive *prim, const Scene &scene, const Ray shadow_ray) const {
        const float shadow_len = length(shadow_ray.dir);

        // The opacity of all objects on the way to the light determines
        // how much light gets through to this object.
        vector<Intersection> all_intersections = scene.all_intersections(shadow_ray, prim);
        // The multiplication of all transparencies of all objects (before
        // the light) along the ray.
        float acc_mult_transparency = 1.0f;

        for (size_t i=0; i<all_intersections.size() && acc_mult_transparency >= 0.001; i++) {
            // If the intersection occurs between the light and this object.
            if (length(all_intersections[i].pos - shadow_ray.start) < shadow_len) {
                acc_mult_transparency *= all_intersections[i].primitive->shader->transparency();
            }
        }

        return acc_mult_transparency;
    }
};

#endif // SHADER_H
