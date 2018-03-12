#include "primitive.h"

#ifndef TORUS_H
#define TORUS_H

class Torus: public Primitive {
    Torus(const Shader *shader, BoundingCube bounding_cube):
        shader(shader), bounding_cube(bounding_cube) {
    };

    // return: the position of the intersection with the primitive, or nullptr
    //		   if no intersection occurred.
    virtual optional<vec4> intersection(const Ray &ray) const = 0;

    // return: the normal to the primtive at the given point on the primitive.
    virtual vec4 normal_at(vec4 point) const = 0;

private:
    // return: the minimum and maximum corners of bounding box around the torus.
    static BoundingCube make_bounding_cube(vec4 center, float radius) {
        // TODO
        return BoundingCube(vec4(), vec4());
    }
};

#endif // TORUS_H
