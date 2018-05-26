#pragma once

#include <glm/gtc/random.hpp>
#include <math.h>

using glm::vec2;
using glm::vec3;

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
    vec3 p = glm::ballRand(max_radius);
    return vec3(p.x + center.x, p.y + center.y, p.z+ center.z);
}
