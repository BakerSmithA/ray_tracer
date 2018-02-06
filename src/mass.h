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

    // return: the angle a ray is deflected given the closest distance from the
    //         ray to the center of mass. The deflection angle is inversely
    //         proportional to the distance.
    float deflection_angle(float closest_distance) {
        // Using the forumla found here:
        //  https://en.wikipedia.org/wiki/Gravitational_lens
        return this->mass / (closest_distance * closest_distance);
    }
};

#endif // MASS_H
