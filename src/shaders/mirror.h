#include <glm/glm.hpp>

using glm::dot;

#ifndef MIRROR_H
#define MIRROR_H

// Models an ideally specular surface, reflecting 100% of light the hits the surface.
class Mirror: public Shader {
public:
    // return: the color of the intersected surface, as illuminated by a
    //         specific light. Becomes the color of point the bounced ray
    //         intersects with.
    vec3 color(vec4 position, vec4 surface_normal, const Ray &incoming, const Scene &scene, const Light &light) const {
        vec4 outgoing_dir = 2.0f * dot(incoming.dir, surface_normal) * (surface_normal - incoming.dir);
        return vec3(0,0,0);
    }
};

#endif // MIRROR_H
