#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "ray.h"

using glm::vec4;
using glm::mat4;
using std::cout;
using std::endl;

class BoundingCube {
public:
    const vec4 min, max;
    const vec4 center;

    BoundingCube(vec4 min, vec4 max): min(min), max(max), center((max + min) * 0.5f) {
    }

    // return: whether the ray intersects the object. Used to reduce the number
    //         of ray-primitive calculations that need to be done.
    bool does_intersect_ray(const Ray &ray) const {
        float tx1 = (this->min.x - ray.start.x) * ray.inv_dir.x;
        float tx2 = (this->max.x - ray.start.x) * ray.inv_dir.x;

        float tmin = std::min(tx1, tx2);
        float tmax = std::max(tx1, tx2);

        float ty1 = (this->min.y - ray.start.y) * ray.inv_dir.y;
        float ty2 = (this->max.y - ray.start.y) * ray.inv_dir.y;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        return tmax >= tmin;
    }
};
