#include <glm/glm.hpp>
#include "ray.h"

using glm::mat3;
using glm::inverse;

#ifndef INTERSECTION_H
#define INTERSECTION_H

class Primitive;

class Intersection {
public:
    // The position, in camera coordinates, of the intersection.
    const vec4 pos;
    // The triangle that was intersected with.
    const Primitive *primitive;

    Intersection(vec4 pos, const Primitive *primitive): pos(pos), primitive(primitive) {
    }
};

#endif // INTERSECTION_H
