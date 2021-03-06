#pragma once

#include <glm/glm.hpp>
#include "ray_spawner.h"

using glm::dot;

// Models an ideally specular surface, reflecting 100% of light the hits the surface.
class Mirror: public RaySpawner {
public:
    // return: the direction of the reflected incoming ray.
    virtual vec4 outgoing_ray_dir(const vec4 position, const Primitive *prim, const Ray &incoming) const override {
        vec4 incident_ray = -incoming.dir;
        vec4 normal = prim->normal_at(position);
        return 2.0f * dot(incident_ray, normal) * normal - incident_ray;
    }
};
