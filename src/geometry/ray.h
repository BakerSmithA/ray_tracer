#include <glm/glm.hpp>

using glm::vec4;
using std::runtime_error;

#ifndef RAY_H
#define RAY_H

class Ray {
public:
    // The start position of the ray.
    const vec4 start;
    // The direction of the ray.
    const vec4 dir;
    // The number of bounces the ray has remaining. Once this reaches zero the
    // ray should no longer be used.
    const int bounces_remaining;
    // Whether the ray is travelling in a vacuum, as opposed to another medium
    // such as glass. This is used for refraction calculations.
    const bool is_in_vacuum;

    Ray(vec4 start, vec4 dir, int bounces_remaining, bool is_in_vacuum = true):
        start(start), dir(dir), bounces_remaining(bounces_remaining), is_in_vacuum(is_in_vacuum)
    {
        if (bounces_remaining < 0) {
            throw runtime_error("Rays should not have negative bounces");
        }
    }

    // return: whether the ray can bounce anymore times.
    bool can_bounce() const {
        return bounces_remaining != 0;
    }
};

#endif // RAY_H
