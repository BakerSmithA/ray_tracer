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

        vec3 normal_3d = normalize(vec3(tri.normal));
        vec3 incoming_3d = normalize(vec3(incoming.dir));

        // N x i = cos(theta_1)
        float a = dot(normal_3d, incoming_3d);
        // 1 - cos(theta_1)^2
        float b = 1 - a * a;
        //
        float c = this->ray_velocity_ratio * this->ray_velocity_ratio * b;
        //
        float d = sqrt(1 - c);
        //
        float e = this->ray_velocity_ratio * a;
        //
        vec3  f = (e - d) * normal_3d;
        //
        vec3  g = this->ray_velocity_ratio * incoming_3d + f;

        vec4 outgoing_dir = project_to_4D(g);
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

// // From vector form of Snell's law from:
// //  http://www.starkeffects.com/snells-law-vector.shtml
// vec3 normal_3d = normalize(vec3(tri.normal));
// vec3 incoming_3d = normalize(vec3(incoming.dir));
//
// vec3  a = cross(-normal_3d, incoming_3d);
// vec3  b = this->ray_velocity_ratio * cross(normal_3d, a);
//
// vec3  c = cross(normal_3d, incoming_3d);
// float d = dot(c, c);
// float e = 1 - (this->ray_velocity_ratio * this->ray_velocity_ratio) * d;
// vec3  f = normal_3d * sqrt(e);
//
// vec4 outgoing_dir = project_to_4D(b - f);
// Ray refracted_outgoing = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);
//
// unique_ptr<Intersection> i = scene.closest_intersection(refracted_outgoing, &tri);
// if (!i) {
//     return vec3(0, 0, 0);
// }
//
// return i->triangle.shader->shadowed_color(i->pos, i->triangle, refracted_outgoing, scene, light);

#endif // REFRACTION_H
