#include <glm/glm.hpp>
#include <iostream>

using glm::vec4;
using glm::mat4;
using std::cout;
using std::endl;

#ifndef BOUNDING_CUBE_H
#define BOUNDING_CUBE_H

class BoundingCube {
public:
    const vec4 min, max;

    BoundingCube(vec4 min, vec4 max): min(min), max(max) {
    }
};

#endif // BOUNDING_CUBE_H
