#include <glm/glm.hpp>
#include <stdexcept>

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
    // The inverse of the direction ray, i.e. 1/dir. This is precalculated for
    // the ray-bounding cube collision calculations.
    const vec4 inv_dir;
    // The normalised direction. The is precalculated here to avoid it being
    // recalculated later.
    const vec4 normalized_dir;
    // The number of bounces the ray has remaining. Once this reaches zero the
    // ray should no longer be used.
    const int bounces_remaining;

    Ray(vec4 start, vec4 dir, int bounces_remaining):
        start(start), dir(dir), inv_dir(1.0f/dir), normalized_dir(glm::normalize(dir)), bounces_remaining(bounces_remaining)
    {
        if (bounces_remaining < 0) {
            throw std::runtime_error("Rays should not have negative bounces");
        }
    }

    // return: whether the ray can bounce anymore times.
    bool can_bounce() const {
        return bounces_remaining != 0;
    }

    // return: this ray, where the start position is offset by a small amount.
    Ray offset(vec4 offset_dir, float offset_scalar) {
        return Ray(this->start + offset_scalar * offset_dir, this->dir, this->bounces_remaining);
    }
};

#endif // RAY_H
