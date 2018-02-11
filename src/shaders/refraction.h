#include <glm/glm.hpp>
#include <math.h>
#include "../geometry/projection.h"

using glm::vec3;
using glm::cross;
using glm::dot;

#ifndef REFRACTION_H
#define REFRACTION_H

// Models an ideal refraction surface, i.e. 100% of the light entering is
// refracted.
class Refraction: public Shader {
public:
    // The ratio n1/n2, assuming n1 is a vacuum. This indicates how much light
    // slows down when entering the medium.
    float ray_velocity_ratio;

    Refraction(float ray_velocity_ratio): ray_velocity_ratio(ray_velocity_ratio) {
    }

    // return: the color of the intersected surface, as illuminated by a
    //         specific light. Becomes the color of the point that the
    //         refracted ray hits. If the ray has no bounces remaining, then
    //         black is returned.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, const Light &light) const override {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }

        float refraction_index = this->ray_velocity_ratio;

        vec3 normal_3d = normalize(vec3(tri.normal));
        vec3 incoming_3d = normalize(vec3(incoming.dir));

        // N x i = cos(theta_1)
        float a = -dot(normal_3d, incoming_3d);

        // According to Wikipedia, if dot product is negative, flip normal and recalculate
        // Captures when the light is going out of the glass (when angle between normal and refracted ray is larger than 90)
        if (a < 0) {
            normal_3d = -normal_3d;
            a = -dot(normal_3d, incoming_3d);
        }

        // 1 - cos(theta_1)^2
        float b = 1 - a * a;
        //
        float c = refraction_index * refraction_index * b;
        //
        float d = sqrt(1 - c);
        //
        float e = refraction_index * a;
        //
        vec3  f = (e - d) * normal_3d;
        //
        vec3  g = refraction_index * incoming_3d + f;

        vec4 outgoing_dir = project_to_4D(g);
        // Assume there is only two states for the ray, in or not in a vacuum.
        // Therefore, if changes state when it crosses the boundary.
        Ray refracted_outgoing = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);

        unique_ptr<Intersection> i = scene.closest_intersection(refracted_outgoing, &tri);
        if (!i) {
            return vec3(0, 0, 0);
        }

        return i->triangle.shader->shadowed_color(i->pos, i->triangle, refracted_outgoing, scene, light);
    }

    bool is_transparent() const override {
        return true;
    }
};

#endif // REFRACTION_H
