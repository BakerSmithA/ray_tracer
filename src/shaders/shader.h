#include "../geometry/scene.h"

#ifndef SHADER_H
#define SHADER_H

// Used to model different types of surfaces, e.g. matte, glossy, etc.
class Shader {
public:
    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(const vec4 position, const Primitive &primitive, const Ray &incoming, const Scene &scene, const Light &light) const = 0;

    // return: the proportion by which light is let through the
    //         material. E.g. a value of 1 is totally transparent, and a value
    //         of 0 is totally opaque.
    virtual float transparency() const {
        return 0.0f;
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
            const float shadow_len = length(shadow_ray.dir);

            // The opacity of all objects on the way to the light determines
            // how much light gets through to this object.
            vector<Intersection> all_intersections = scene.all_intersections(shadow_ray, &prim);
            // The multiplication of all transparencies of all objects (before
            // the light) along the ray.
            float acc_transparency = 1.0f;

            for (size_t i=0; i<all_intersections.size() && acc_transparency >= 0.001; i++) {
                // If the intersection occurs between the light and this object.
                if (length(all_intersections[i].pos - shadow_ray.start) < shadow_len) {
                    acc_transparency *= all_intersections[i].primitive.shader->transparency();
                }
            }

            return acc_transparency * col;
        }

        return col;
    }
};

#endif // SHADER_H
