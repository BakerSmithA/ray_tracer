#include "shader.h"
#include "../textures/texture_3d.h"

#ifndef VOLUMETRIC_H
#define VOLUMETRIC_H

// Represents a material which light can penetrate, e.g. smoke.
// Uses ray marching to sample the interior structure of the volume.
// Lighting is calculated using nested ray marching.
class Volumetric: public Shader {
public:
    // Describes the shape, color, and transparency of the 3d texture.
    Texture3d *texture;
    // The size of the steps to use for primary ray marching inside the volume.
    const float primary_ray_step_size;
    // The size of the steps to use when calculating lighting inside the volume.
    const float shadow_ray_step_size;
    // Attenuation due of light to both absorption and scatterring.
    const float extinction_coefficient;

    Volumetric(Texture3d *texture, float primary_ray_step_size, float shadow_ray_step_size, float extinction_coefficient):
        texture(texture),
        primary_ray_step_size(primary_ray_step_size),
        shadow_ray_step_size(shadow_ray_step_size),
        extinction_coefficient(extinction_coefficient)
    {
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
        // furthest a ray will travel inside the volume.
        // This is offset from the initial intersection position, since we
        // cannot exclude this primitive from the search, e.g. for spheres.
        const float offset_dist = this->primary_ray_step_size * -0.001;
        Ray outgoing = Ray(position, incoming.dir, incoming.bounces_remaining - 1)
                      .offset(prim->normal_at(position), offset_dist);

        // Find the how much light made it through the volume, starting with
        // full transparency.
        float shadow_extinction = 1.0f;

        // Calculate how much light intensity is lost as the light travels
        // through the volume.
        auto shadow_ray_step = [&](vec4 step_pos, float step_size) {
            // The projection of the step position into object coordinates.
            vec4 proj = prim->parent_obj->converted_world_to_obj(step_pos);
            float density = this->texture->density_at(proj);

            // Find how much light reached the position from the light.
            //float lighting = mean_random_transparency(termination_pos, prim, scene, light, num_shadow_rays);

            shadow_extinction *= exp(-this->extinction_coefficient * density * step_size);
        };

        // Find the how much light made it through the volume, starting with
        // full transparency.
        float extinction = 1.0f;

        // Used to update the extinction for how much light enters the camera.
        auto primary_ray_step = [&](vec4 step_pos, float step_size) {
            // The projection of the step position into object coordinates.
            vec4 proj = prim->parent_obj->converted_world_to_obj(step_pos);
            float density = this->texture->density_at(proj);

            // Calculate how much light reaches the position in the volume from
            // where the shadow ray stops. This may be at the edge of the
            // volume or at an object inside the volume.
            optional<Ray> shadow_ray = light.ray_from(step_pos);
            if (shadow_ray.has_value()) {
                Ray offset_shadow_ray = shadow_ray.value().offset(prim->normal_at(step_pos), offset_dist);
                this->for_each_ray_step(step_pos, offset_shadow_ray, this->shadow_ray_step_size, scene, shadow_ray_step);
            }

            extinction *= exp(-this->extinction_coefficient * density * step_size);
        };

        this->for_each_ray_step(position, outgoing, this->primary_ray_step_size, scene, primary_ray_step);

        // If the extinction is too low, there's no point computing the
        // background color as it will not show through.
        if (extinction < 0.001f) {
            return vec3(0.0f, 0.0f, 0.0f);
        }

        // Compute the color of the object behind the volume, to mix with the
        // volume color.
        vec3 background_col = this->color_behind(position, prim, outgoing, scene, light, num_shadow_rays);

        return glm::mix(vec3(shadow_extinction), background_col, extinction);
    }

private:
    // param f: called for each step and given the position of the step,
    //          and the step size for that step.
    // effect: performs ray marching along the ray, running the function f at
    //         step along the ray until the ray exits the volume or hits an
    //         object inside the volume.
    void for_each_ray_step(const vec4 position, const Ray through_vol_ray, const float max_step_size, const Scene &scene, function<void(vec4, float)> f) const {
        // Find where the ray exits the volume, or where the ray hits an object
        // inside the volume. Therefore, we know where to stop ray marching.
        optional<Intersection> termination = scene.closest_intersection(through_vol_ray);

        // The ray never came out of the volume.
        if (!termination.has_value()) {
            printf("WARNING: Ray should exit volume, or intersect another object inside\n");
            return;
        }

        const vec4 termination_pos = termination.value().pos;
        const float max_dist = length(position - termination_pos);

        // Defined because we need to perform a final fractional step of any
        // remaining depth to avoid slicing artefacts when other objects are
        // inside the volume.
        float dist = 0.0f;

        // Perform ray marching through the volume. Minus the step size from the
        // maximum to avoid the ray going outside the shape.
        for (; dist < max_dist; dist += max_step_size) {
            vec4 pos = position + through_vol_ray.normalized_dir * dist;
            f(pos, max_step_size);
        }

        // Fractional step to remove slicing artefacts from objects inside volume.
        float fractional_dist = max_dist - dist;
        f(termination_pos, fractional_dist);
    }

    // return: the color of the object behind or inside the volume.
    vec3 color_behind(vec4 position, const Primitive *prim, const Ray &through_vol_ray, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        if (!through_vol_ray.can_bounce()) {
            return vec3(0, 0, 0);
        }
        // The number of bounces is reduced due to this interaction.
        Ray outgoing_ray = Ray(position, through_vol_ray.dir, through_vol_ray.bounces_remaining - 1);

        optional<Intersection> i = scene.closest_intersection(outgoing_ray, prim);
        if (!i.has_value()) {
            return vec3(0, 0, 0);
        }

        return i->primitive->shader->shadowed_color(i->pos, i->primitive, outgoing_ray, scene, light, num_shadow_rays);
    }

    // return: the proportion by which light is let through the
    //         material. E.g. a value of 1 is totally transparent, and a value
    //         of 0 is totally opaque.
    virtual float transparency() const {
        return 1.0f;
    }
};

#endif // VOLUMETRIC_H
