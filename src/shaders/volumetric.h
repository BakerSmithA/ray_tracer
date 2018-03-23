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
    vec3 shadowed_color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // References:
        //  - http://patapom.com/topics/Revision2013/Revision%202013%20-%20Real-time%20Volumetric%20Rendering%20Course%20Notes.pdf
        //  - http://shaderbits.com/blog/creating-volumetric-ray-marcher

        if (!incoming.can_bounce()) {
            return vec3(0.0f, 0.0f, 0.0f);
        }

        // Construct a new ray inside the volume, which is used to test the
        // furthest a ray will travel inside the smoke.
        // This is offset from the initial intersection position, since we
        // cannot exclude this primitive from the search, e.g. for spheres.
        const float offset_dist = this->ray_step_size * -0.001;
        Ray outgoing = Ray(position, incoming.dir, incoming.bounces_remaining - 1)
                      .offset(prim->normal_at(position), offset_dist);

        // Find the how much light made it through the volume, starting with
        // full transparency.
        float extinction = 1.0f;

        auto step = [&](vec4 step_pos) {
            float density = this->texture->density_at(step_pos);
            extinction *= exp(-this->extinction_coefficient * density * this->ray_step_size);
        };

        this->for_each_ray_step(outgoing, scene, step);

        // If the extinction is too low, there's no point computing the
        // background color as it will not show through.
        if (extinction < 0.001f) {
            return vec3(0.0f, 0.0f, 0.0f);
        }

        // Compute the color of the object behind the smoke, to mix with the
        // smoke color.
        vec3 background_col = this->color_behind(prim, outgoing, scene, light, num_shadow_rays);

        return glm::mix(vec3(0,0,0), background_col, extinction);
    }

private:
    // param f: called for each step and given the position of the step.
    // effect: performs ray marching along the ray, running the function f at
    //         step along the ray until the ray exits the volume or hits an
    //         object inside the volume.
    void for_each_ray_step(const Ray ray, const Scene &scene, function<void(vec4)> f) const {
        // Find where the ray exits the smoke, or where the ray hits an object
        // inside the smoke. Therefore, we know where to stop ray marching.
        const float max_dist = this->distance_in_volume(ray.start, ray, scene);

        // Perform ray marching through the volume.
        for (float dist = 0.0f; dist <= max_dist; dist += this->ray_step_size) {
            vec4 pos = ray.start + ray.normalized_dir * dist;
            f(pos);
        }
    }

    // return: The distance the ray travels in the smoke before exiting.
    float distance_in_volume(const vec4 intersection_pos, const Ray outgoing, const Scene &scene) const {
        optional<Intersection> collision = scene.closest_intersection(outgoing);

        // The ray never came out of the volume.
        if (!collision.has_value()) {
            return 0.0f;
        }

        return length(intersection_pos - collision.value().pos);
    }

    // return: the color of the object behind or inside the smoke.
    vec3 color_behind(const Primitive *prim, const Ray &outgoing, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // Don't include the volumetric object in the search.
        optional<Intersection> op_collision = scene.closest_intersection_excluding_obj(outgoing, prim->parent_obj);

        // The ray never came out of the volume.
        if (!op_collision.has_value()) {
            return vec3(0, 0, 0);
        }

        Intersection collision = op_collision.value();
        return collision.primitive->shader->shadowed_color(collision.pos, prim, outgoing, scene, light, num_shadow_rays);
    }
};

#endif // VOLUMETRIC_H
