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
    // Color of light exiting the volume.
    const vec3 extinction_color;
    // The size of the steps to use for primary ray marching inside the volume.
    const float primary_ray_step_size;
    // The size of the steps to use when calculating lighting inside the volume.
    const float shadow_ray_step_size;
    // Attenuation due of light to both absorption and scatterring.
    const float extinction_coefficient;
    // Measure of particles being able to scatter light out of a beam.
    const float scattering_coefficient;
    // The multiplier the step size for offsetting rays to be inside the volume.
    const float offset_multipler = -0.001f;

    Volumetric(Texture3d *texture, vec3 extinction_color, float primary_ray_step_size, float shadow_ray_step_size, float extinction_coefficient, float scattering_coefficient):
        texture(texture),
        extinction_color(extinction_color),
        primary_ray_step_size(primary_ray_step_size),
        shadow_ray_step_size(shadow_ray_step_size),
        extinction_coefficient(extinction_coefficient),
        scattering_coefficient(scattering_coefficient)
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
        const float offset_dist = this->primary_ray_step_size * this->offset_multipler;
        Ray through_vol_ray = Ray(position, incoming.dir, incoming.bounces_remaining - 1)
                             .offset(prim->normal_at(position), offset_dist);

        vec4 out_col = this->ray_marched_color(position, through_vol_ray, prim, scene, light);

        // Compute the color of the object behind the volume, to mix with the
        // volume color.
        vec3 background_col = this->color_behind(position, prim, through_vol_ray, scene, light, num_shadow_rays);

        return glm::mix(vec3(out_col), background_col, out_col.w);
    }

private:
    // return: the color, including alpha, of the volume at the given position
    //         on the surface.
    //         Calculated by performs ray marching through the volume to
    //         determine the interior structure. Also computes the lighting for
    //         points inside the volume.
    vec4 ray_marched_color(vec4 position, Ray through_vol_ray, const Primitive *prim, const Scene &scene, const Light &light) const {
        // Start off with full transparency as the light has not yet passed through any volume.
        float extinction = 1.0f;
        // Start with no accumulated color, as the light has not yet passed through any volume.
        vec3 out_col = vec3(0.0f, 0.0f, 0.0f);

        auto primary_ray_step = [&](vec4 step_pos, float step_size) {
            vec4 proj = prim->parent_obj->converted_world_to_obj(step_pos);
            float density = this->texture->density_at(proj);

            extinction *= exp(-this->extinction_coefficient * density * step_size);
            vec3 light_col = this->scattered_light_color(step_pos, prim, scene, light);
            vec3 step_scattering = light_col * step_size;
            out_col += extinction * step_scattering;

            // Stop marching is the extinction is low enough.
            return extinction >= 0.001f;
        };

        // Ray march through the volume.
        this->for_each_ray_step(position, through_vol_ray, this->primary_ray_step_size, scene, primary_ray_step);

        return vec4(out_col, extinction);
    }

    // return: the color of the light at the given position inside the volume.
    //         Takes scattering of the light into account as it goes through
    //         the volume.
    vec3 scattered_light_color(vec4 position, const Primitive *prim, const Scene &scene, const Light &light) const {
        optional<Ray> shadow_ray = light.ray_from(position);

        // If the light does not cast shadows, then the light cannot be
        // scattered as it moves through the volume.
        if (!shadow_ray.has_value()) {
            return light.color;
        }

        // Find the how much light made it through the volume, starting with
        // full transparency.
        vec3 extinction = vec3(1.0f);
        auto shadow_ray_step = [&](vec4 step_pos, float step_size) {
            vec4 proj = prim->parent_obj->converted_world_to_obj(step_pos);
            float density = this->texture->density_at(proj);
            vec3 opposite_extinction_color = vec3(1.0f) - this->extinction_color;
            extinction *= glm::exp(-this->extinction_coefficient * density * step_size * opposite_extinction_color);

            return true;
        };

        // Offset the shadow ray to ensure it's inside the volume.
        Ray offset_shadow_ray = shadow_ray.value().offset(prim->normal_at(position), this->shadow_ray_step_size * this->offset_multipler);
        // Ray march through the volume.
        this->for_each_ray_step(position, offset_shadow_ray, this->shadow_ray_step_size, scene, shadow_ray_step);

        return light.color * extinction * this->scattering_coefficient;
    }

    // param f: called for each step and given the position of the step,
    //          and the step size for that step.
    // effect: performs ray marching along the ray, running the function f at
    //         step along the ray until the ray exits the volume or hits an
    //         object inside the volume.
    void for_each_ray_step(const vec4 position, const Ray through_vol_ray, const float max_step_size, const Scene &scene, function<bool(vec4, float)> f) const {
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
        // Whether to continue marching through the volume. This is updated
        // by the function which marches through the volume, therefore allowing
        // for time savings.
        bool should_march = true;

        // Perform ray marching through the volume. Minus the step size from the
        // maximum to avoid the ray going outside the shape.
        for (; dist < max_dist && should_march; dist += max_step_size) {
            vec4 pos = position + through_vol_ray.normalized_dir * dist;
            should_march = f(pos, max_step_size);
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
    //         material. The value is the extinction and is calculated by
    //         performing ray marching through the volume.
    virtual float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        float extinction = 1.0f;

        auto ray_step = [&](vec4 step_pos, float step_size) {
            vec4 proj = prim->parent_obj->converted_world_to_obj(step_pos);
            float density = this->texture->density_at(proj);
            extinction *= exp(-this->extinction_coefficient * density * step_size);

            // Stop marching is the extinction is low enough.
            return extinction >= 0.001f;
        };

        // Offset the shadow ray to ensure it's inside the volume.
        Ray offset_shadow_ray = Ray(position, shadow_ray.dir, 0).offset(prim->normal_at(position), this->shadow_ray_step_size * this->offset_multipler);
        this->for_each_ray_step(position, offset_shadow_ray, this->shadow_ray_step_size, scene, ray_step);

        return extinction;
    }
};

#endif // VOLUMETRIC_H
