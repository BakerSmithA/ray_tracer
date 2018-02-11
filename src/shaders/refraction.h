#include <glm/glm.hpp>
#include <math.h>
#include "../geometry/projection.h"
#include "ray_spawner.h"

using glm::vec3;
using glm::cross;
using glm::dot;

#ifndef REFRACTION_H
#define REFRACTION_H

// Models an ideal refraction surface, i.e. 100% of the light entering is
// refracted.
class Refraction: public RaySpawner {
public:
    // The ratio n1/n2, assuming n1 is a vacuum. This indicates how much light
    // slows down when entering the medium.
    float ray_velocity_ratio;

    Refraction(float ray_velocity_ratio): ray_velocity_ratio(ray_velocity_ratio) {
    }

    // return: the direction of the refracted incoming ray.
    vec4 outgoing_ray_dir(const Triangle &tri, const Ray &incoming) const override {
        float refraction_index = this->ray_velocity_ratio;

        vec3 normal_3d = normalize(vec3(tri.normal));
        vec3 incoming_3d = normalize(vec3(incoming.dir));

        // N x i = cos(theta_1)
        float a = -dot(normal_3d, incoming_3d);

        // According to Wikipedia, if dot product is negative, flip normal and recalculate.
        // This captures when the light is going out of the glass (when angle
        // between normal and refracted ray is larger than 90).
        if (a < 0) {
            normal_3d = -normal_3d;
            a = -dot(normal_3d, incoming_3d);
        }

        // 1 - cos(Θ)^2
        float b = 1 - a * a;
        // r^2 * (1 - cos(Θ)^2)
        float c = refraction_index * refraction_index * b;
        // sqrt(1 - r^2 * (1 - cos(Θ)^2))
        float d = sqrt(1 - c);
        // r * cos(Θ)
        float e = refraction_index * a;
        // N * ((r * cos(Θ)) - (sqrt(1 - r^2 * (1 - cos(Θ)^2))))
        vec3 f = (e - d) * normal_3d;
        // rI + N * ((r * cos(Θ)) - (sqrt(1 - r^2 * (1 - cos(Θ)^2))))
        vec3 g = refraction_index * incoming_3d + f;

        return project_to_4D(g);
    }

    bool is_transparent() const override {
        return true;
    }
};

#endif // REFRACTION_H
