#pragma once

#include <optional>

using std::optional;

// Models a surface who's color is found by firing out another ray, e.g. mirror.
class RaySpawner: public ShadowedShader {
    // return: the ray direction used to find the color of the shader, e.g. the
    //         reflected ray for a mirror.
    virtual vec4 outgoing_ray_dir(const vec4 position, const Primitive *prim, const Ray &incoming) const = 0;

    // return: the color of the shader, determined by shooting another ray into
    //         the scene. Or, black if the incoming ray cannot bounce anymore.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }

        vec4 outgoing_dir = this->outgoing_ray_dir(position, prim, incoming);
        // The number of bounces is reduced due to this interaction.
        Ray outgoing_ray = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);

        optional<Intersection> i = scene.closest_intersection(outgoing_ray, prim);
        if (!i.has_value()) {
            return vec3(0, 0, 0);
        }

        return i->primitive->shader->shadowed_color(i->pos, i->primitive, outgoing_ray, scene, light, num_shadow_rays);
    }

    // return: the color of the object in ambient lighting conditions, i.e.
    //         with no shadows.
    vec3 ambient_color(vec4 position, const Primitive *prim, const Light &light) const {
        return vec3(0, 0, 0);
    }
};
