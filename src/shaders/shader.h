#include "../geometry/scene.h"

#ifndef SHADER_H
#define SHADER_H

// Used to model different types of surfaces, e.g. matte, glossy, etc.
class Shader {
public:
    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, const Light &light) const = 0;

    // return: whether the shader is transparent, i.e. doesn't block shadows.
    virtual bool is_transparent() const = 0;

    // return: the color of the intersected surface, taking shadows from the
    //         light into account. If the position is in shadow, black is
    //         returned, otherwise the shader is used to calculate color.
    vec3 shadowed_color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, const Light &light) const {
        // Work out whether we cast shadows, and if we do, whether the intersection
        // point is in shadow.
        if (light.does_cast_shadows()) {
            // If the shadow ray between the intersection and the light is
            // obstructed, no light from this light reaches the intersection.
            Ray shadow_ray = light.shadow_ray_to(position);
            // Exclude the current triangle from obstruction calculations.
            if (scene.is_obstructed(shadow_ray, &Shader::is_transparent, tri)) {
                return vec3(0, 0, 0);
            }
        }

        return this->color(position, tri, incoming, scene, light);
    }
};

#endif // SHADER_H
