#ifndef LIGHT_H
#define LIGHT_H

// Models a light source.
class Light {
public:
    const vec3 color;

    Light(vec3 color): color(color) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    virtual vec3 intensity(vec4 point, vec4 surface_normal) const = 0;
};

#endif // LIGHT_H
