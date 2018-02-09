#include <glm/glm.hpp>
#include "triangle.h"
#include <limits.h>

#ifndef SCENE_H
#define SCENE_H

// Contains all the geometry, lights, etc for a scene.
class Scene {
private:
    vector<Triangle> &triangles;

public:
    Scene(vector<Triangle> &triangles): triangles(triangles) {
    }

    // param excluded_triangle: the triangle to discount intersections with.
    //                          This can be useful to avoid self-intersection.
    // param intersection:      Populated with the closest intersection to the
    //                          start of the ray.
    // return:                  Whether an intersection was found.
    bool closest_intersection(Ray &ray, Intersection *intersection, Triangle *excluded_triangle = NULL) {
        float closest_distance = std::numeric_limits<float>::max();
        bool found = false;
    }
};

#endif // SCENE_H
