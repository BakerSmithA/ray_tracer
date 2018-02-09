#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;

#ifndef PROJECTION_H
#define PROJECTION_H

// returns: the result of projecting a 3D point into the 3D plane in 4D space.
vec4 project_to_4D(vec3 vec) {
    return vec4(vec.x, vec.y, vec.z, 1.0f);
}

#endif // PROJECTION_H
