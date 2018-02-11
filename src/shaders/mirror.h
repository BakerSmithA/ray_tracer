#include <glm/glm.hpp>
#include "ray_spawner.h"

using glm::dot;
using std::unique_ptr;

#ifndef MIRROR_H
#define MIRROR_H

// Models an ideally specular surface, reflecting 100% of light the hits the surface.
class Mirror: public RaySpawner {
public:
    // return: the direction of the reflected incoming ray.
    vec4 outgoing_ray_dir(const Triangle &tri, const Ray &incoming) const override {
        vec4 incident_ray = -incoming.dir;
        return 2.0f * dot(incident_ray, tri.normal) * tri.normal - incident_ray;
    }

    bool is_transparent() const override {
        return false;
    }
};

#endif // MIRROR_H