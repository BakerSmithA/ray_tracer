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

    Volumetric(Texture<vec4> *texture, float ray_step_size):
        texture(texture), ray_step_size(ray_step_size) {
    }

    // return: the color of the volume by performing ray marching though the object.
    vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // Initial color is color of wherever the ray hit the primitive.
        vec3 out_col = this->texture->color_at(position);

        // Find where the ray exits the smoke, or where the ray hits an object
        // inside the smoke. Therefore, we know where to stop ray marching.
        const float dist = this->distance_in_volume(position, incoming, prim, scene);

        return out_col * dist;
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
};

#endif // VOLUMETRIC_H
