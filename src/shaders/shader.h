#include "../geometry/scene.h"

#ifndef SHADER_H
#define SHADER_H

// Used to model different types of surfaces, e.g. matte, glossy, etc.
class Shader {
public:
    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(const vec4 position, const Primitive &primitive, const Ray &incoming, const Scene &scene, const Light &light) const = 0;

    // return: the proportion by which light is blocked from going through the
    //         material. E.g. a value of 1 totally blocks light.
    virtual float opacity() const {
        return 1.0;
    }

    // return: the color of the intersected surface, taking shadows from the
    //         light into account. If the position is in shadow, black is
    //         returned, otherwise the shader is used to calculate color.
    virtual vec3 shadowed_color(vec4 position, const Primitive &prim, const Ray &incoming, const Scene &scene, const Light &light) const {
        const vec3 col = this->color(position, prim, incoming, scene, light);

        // Work out whether we cast shadows, and if we do, whether the intersection
        // point is in shadow.
        if (light.does_cast_shadows()) {
            // If the shadow ray between the intersection and the light is
            // obstructed, no light from this light reaches the intersection.
            Ray shadow_ray = light.shadow_ray_to(position);

            // The opacity of the object that was intersected determines how
            // much light hits this surface.
            unique_ptr<Intersection> intersected = scene.closest_intersection(shadow_ray, &prim);
            // The intersection must occur in front of the light.
            if (intersected != nullptr && length(intersected->pos - shadow_ray.start) < length(shadow_ray.dir)) {
                return (1 - intersected->primitive.shader->opacity()) * col;
            }
        }

        return col;
    }
};

#endif // SHADER_H
