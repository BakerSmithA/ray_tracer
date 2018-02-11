#include <glm/glm.hpp>

using glm::dot;
using std::unique_ptr;

#ifndef MIRROR_H
#define MIRROR_H

// Models an ideally specular surface, reflecting 100% of light the hits the surface.
class Mirror: public Shader {
public:
    // return: the color of the intersected surface, as illuminated by a
    //         specific light. Becomes the color of point the bounced ray
    //         intersects with. If the incoming ray has no bounces remaining,
    //         black is returned.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, const Light &light) const override {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }

        vec4 incident_ray = -incoming.dir;
        vec4 outgoing_dir = 2.0f * dot(incident_ray, tri.normal) * tri.normal - incident_ray;
        // The outgoing ray has one less bounce remaining than the incoming ray.
        Ray reflected_outgoing = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);

        unique_ptr<Intersection> i = scene.closest_intersection(reflected_outgoing, &tri);
        if (!i) {
            return vec3(0, 0, 0);
        }

        return i->triangle.shader->shadowed_color(i->pos, i->triangle, reflected_outgoing, scene, light);
    }

    bool is_transparent() const override {
        return false;
    }
};

#endif // MIRROR_H
