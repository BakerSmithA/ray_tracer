#pragma once

#include "dist_from_center.h"
#include <math.h>

// A shader which deflects light rays according to an inverse square
// relationship with distance to the center of the object.
class GravitationalLens: public DistFromCenter {
public:
    // param black_out_angle: the which above, the color is black. Can be used
    //                        to create a black hole in the center. The
    //                        distortion angle ranges from 0-pi.
    GravitationalLens(float strength, float max_dist, float black_out_angle=M_PI):
        DistFromCenter(GravitationalLens::bent_ray_color(strength, black_out_angle), 1.0f, max_dist) {
    }

private:
    static ConvertDistToColor bent_ray_color(float strength, float black_out_angle) {
        auto bent_ray = [=](float closest_dist, vec4 projection, vec4 diff, vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) {
            // The angle to deflect the ray towards the center of the lens.
            float angle = strength / (closest_dist * closest_dist);
            float clamped_angle = glm::clamp(angle, 0.0f, (float)M_PI);

            if (clamped_angle > black_out_angle) {
                return vec3(0,0,0);
            }

            vec3 outgoing_dir_3d = deflected(vec3(incoming.dir), clamped_angle, vec3(projection), vec3(diff));
            vec4 outgoing_dir = project_to_4D(outgoing_dir_3d);
            // The number of bounces is reduced due to this interaction.
            Ray outgoing_ray = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);

            optional<Intersection> i = scene.closest_intersection(outgoing_ray, prim);
            if (!i.has_value()) {
                return vec3(0, 0, 0);
            }

            return i->primitive->shader->shadowed_color(i->pos, i->primitive, outgoing_ray, scene, light, num_shadow_rays);
        };

        return bent_ray;
    }
};
