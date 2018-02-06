#include <glm/glm.hpp>

using glm::vec4;

#ifndef MASS_H
#define MASS_H

class Mass {
public:
    const vec4 position;
    const float mass;

    Mass(vec4 position, float mass): position(position), mass(mass) {
    }
};

#endif // MASS_H
