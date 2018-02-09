#include <glm/glm.hpp>
#include "ray.h"
#include "triangle.h"

using glm::mat3;
using glm::inverse;

#ifndef INTERSECTION_H
#define INTERSECTION_H

class Intersection {
public:
    // The position, in camera coordinates, of the intersection.
    const vec4 pos;
    // The triangle that was intersected with.
    const Triangle &triangle;

    Intersection(vec4 pos, Triangle &triangle): pos(pos), triangle(triangle) {
    }

    // return: the intersection [t u v] with the triangle, in the triangle's
    //         coordinates (i.e. e1 and e2 are the basis vectors).
    static vec3 with_triangle(Triangle &tri, Ray &ray) {
        vec3 b = vec3(ray.start - tri.v0);
        mat3 A = mat3(-vec3(ray.dir), tri.e1, tri.e2);
        return inverse(A) * b;
    }
};

#endif // INTERSECTION_H
