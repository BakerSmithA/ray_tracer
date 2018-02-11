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

        // From vector form of Snell's law from:
        //  http://www.starkeffects.com/snells-law-vector.shtml
        // vec3 normal_3d = vec3(tri.normal);
        // vec3 incoming_3d = vec3(incoming.dir);
        //
        // // a = -N x incoming
        // vec3 a = cross(-normal_3d, incoming_3d);
        // // b = (n1/n2)(N x a)
        // vec3 b = this->ray_velocity_ratio * cross(normal_3d, a);
        // // c = N x incoming
        // vec3 c = cross(normal_3d, incoming_3d);
        // // d = c . c
        // float d = dot(c, c);
        // // e = 1 - (n1/n2)^2 * d
        // float e = 1 - (this->ray_velocity_ratio * this->ray_velocity_ratio * d);
        // // f = N * sqrt(e)
        // vec3 f = normal_3d * sqrt(e);

        //vec4 outgoing_dir = project_to_4D(b - f);
        Ray refracted_outgoing = Ray(position, incoming.dir, incoming.bounces_remaining);

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
