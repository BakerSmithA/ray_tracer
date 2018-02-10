#include "../geometry/ray.h"

#ifndef LIGHT_H
#define LIGHT_H

// Models a light source.
class Light {
public:
    enum class LightType { DIFFUSE, SPECULAR, AMBIENT };
    
    const vec3 color;
    const LightType type;
    
    Light(vec3 color, LightType type): color(color), type(type) {}

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of light at the position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const = 0;

    // return: whether the light casts shadows.
    virtual bool does_cast_shadows() const = 0;
};

#endif // LIGHT_H
