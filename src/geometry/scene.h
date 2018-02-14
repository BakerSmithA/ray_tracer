#include <glm/glm.hpp>
#include <limits.h>
#include <vector>
#include "intersection.h"
#include "primitive.h"
#include "projection.h"
#include "../lights/light.h"

using glm::length;
using std::vector;
using std::unique_ptr;

#ifndef SCENE_H
#define SCENE_H

// Contains all the geometry, lights, etc for a scene.
class Scene {
public:
    const vector<Primitive*> primitives;
    const vector<Light*> lights;

    Scene(const vector<Primitive*> primitives, const vector<Light*> lights):
        primitives(primitives), lights(lights)
    {
    }

    // param ray:               A ray, in scene coordinates, check intersection with.
    // param excluded_triangle: the triangle to discount intersections with.
    //                          This can be useful to avoid self-intersection.
    // return:                  The closest intersection to the start of the ray,
    //                          or null if no intersection was found.
    unique_ptr<Intersection> closest_intersection(const Ray &ray, const Primitive *excluded_prim = nullptr) const {
        float closest_distance = std::numeric_limits<float>::max();
        int closest_primitive_idx = -1;
        // The intersection point in the scene's coordinate system.
        vec4 intersection_pos;

        for (size_t i=0; i<this->primitives.size(); i++) {
            if (this->primitives[i] == excluded_prim) {
                continue;
            }

            // The intersection in the scene coordinate system.
            unique_ptr<vec4> intersection = primitives[i]->intersection(ray);

            if (intersection == nullptr) {
                continue;
            }

            // The distance from the point in scene coordinates to the ray in
            // scene coordinates.
            float dist = length(*intersection - ray.start);

            if (dist < closest_distance) {
                closest_distance = dist;
                closest_primitive_idx = i;
                intersection_pos = *intersection;
            }
        }

        if (closest_primitive_idx == -1) {
            return nullptr;
        }

        return unique_ptr<Intersection>(new Intersection(intersection_pos, *primitives[closest_primitive_idx]));
    }

    // param excluded_tri: obstructions by this triangle will not be included.
    //                     This is useful for avoiding self-intersections.
    // return:             whether the ray is obstructed by geometry between its
    //                     start and start + direction.
    bool is_obstructed(const Ray &shadow_ray, bool (Shader::*is_transparent)() const, const Primitive &excluded_prim) const {
        bool obstructed = false;

        for (size_t i=0; i<this->primitives.size() && !obstructed; i++) {
            // The intersection in the triangle's coordinate system.
            unique_ptr<vec4> intersection = primitives[i]->intersection(shadow_ray);

            obstructed = &excluded_prim != primitives[i]
                      && !(primitives[i]->shader->*is_transparent)()
                      && (intersection != nullptr)
                      && length(*intersection - shadow_ray.start) < length(shadow_ray.dir); // The intersection occurred before the end of the ray.
        }

        return obstructed;
    }
};

#endif // SCENE_H
