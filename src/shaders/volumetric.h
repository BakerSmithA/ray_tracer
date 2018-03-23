#include "shader.h"
#include "../textures/texture.h"

#ifndef VOLUMETRIC_H
#define VOLUMETRIC_H

// Represents a material which light can penetrate, e.g. smoke.
class Volumetric: public Shader {
public:
    // Describes the shape, color, and transparency of the 3d texture.
    Texture<vec4> *texture;
    // The size of the steps to use for ray marching inside the object.
    const float ray_step_size;
    // Attenuation due of light to both absorption and scatterring.
    const float extinction_coefficient;

    Volumetric(Texture<vec4> *texture, float ray_step_size, float extinction_coefficient):
        texture(texture), ray_step_size(ray_step_size), extinction_coefficient(extinction_coefficient) {
    }

    // return: the color of the volume by performing ray marching though the object.
    vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // Code with help from pseudo-code here:
        //  http://patapom.com/topics/Revision2013/Revision%202013%20-%20Real-time%20Volumetric%20Rendering%20Course%20Notes.pdf

        // Initial color is color of wherever the ray hit the primitive.
        //vec3 out_col = this->texture->color_at(position);

        // Find where the ray exits the smoke, or where the ray hits an object
        // inside the smoke. Therefore, we know where to stop ray marching.
        const float max_dist = this->distance_in_volume(position, incoming, prim, scene);

        float extinction = 1.0f; // Start with full transparency.
        float scattering = 0.0f; // Start with no accumulated light.

        // Perform ray marching through the volume.
        for (float dist = 0.0f; dist <= max_dist; dist += this->ray_step_size) {
            vec4 pos = position + incoming.normalized_dir * dist;

            // Accumulate extinction for this step, i.e, how much more light has
            // been scattered out of the path of going into the camera.
            float density = this->texture->density_at(pos);
            extinction *= exp(-this->extinction_coefficient * density * this->ray_step_size);
        }

        return vec3(extinction, extinction, extinction);
    }

    // return: the color of the volume by performing ray marching though the object.
    vec3 shadowed_color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        return color(position, prim, incoming, scene, light, num_shadow_rays);
    }

private:
    // return: The distance the ray travels in the smoke before exiting.
    float distance_in_volume(const vec4 intersection_pos, const Ray incoming, const Primitive *prim, const Scene &scene) const {
        // Construct a new ray inside the volume, which is used to test the
        // furthest a ray will travel inside the smoke.
        // This is offset from the initial intersection position, since we
        // cannot exclude this primitive from the search, e.g. for spheres.
        const float offset_dist = this->ray_step_size * -0.001;
        Ray outgoing = Ray(intersection_pos, incoming.dir, incoming.bounces_remaining - 1)
                      .offset(prim->normal_at(intersection_pos), offset_dist);

        optional<Intersection> collision = scene.closest_intersection(outgoing);

        // The ray never came out of the volume.
        if (!collision.has_value()) {
            return 0.0f;
        }

        return length(intersection_pos - collision.value().pos);
    }

    float linear_density() {

    }
};

#endif // VOLUMETRIC_H
