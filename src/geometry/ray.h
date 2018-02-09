#include <glm/glm.hpp>

using glm::vec4;

#ifndef RAY_H
#define RAY_H

class Ray {
public:
    // The start position of the ray.
    const vec4 start;
    // The direction of the ray.
    const vec4 dir;

    Ray(vec4 start, vec4 dir): start(start), dir(dir) {
    }
};

#endif // RAY_H
