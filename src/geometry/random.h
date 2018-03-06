#include <glm/gtc/random.hpp>
#include <math.h>

using glm::vec2;
using glm::vec3;

#ifndef RANDOM_H
#define RANDOM_H

// return: a random point in the the box with the given origin.
vec2 random_in_box(vec2 center, float width, float height) {
    float dx = width / 2;
    float dy = height / 2;

    float x = glm::linearRand(-dx, dx);
    float y = glm::linearRand(-dy, dy);

    return vec2(x + center.x, y + center.y);
}

// return: a random point in or on the sphere.
vec3 random_in_sphere(vec3 center, float max_radius) {
    float pi = (float)M_PI;

    float theta = glm::linearRand(0.0f, 2 * pi);
    float phi   = glm::linearRand(-pi / 2, pi / 2);
    float r     = glm::linearRand(0.0f, max_radius);

    float x = r * cos(theta) * cos(phi);
    float y = r * sin(phi);
    float z = r * sin(theta) * cos(phi);

    return vec3(x + center.x, y + center.y, z + center.z);
}

#endif // RANDOM_H
