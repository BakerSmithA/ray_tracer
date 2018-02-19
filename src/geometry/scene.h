#include <glm/glm.hpp>
#include <limits.h>
#include <vector>
#include <functional>
#include "intersection.h"
#include "primitive.h"
#include "projection.h"
#include "../lights/point_light.h"

using glm::length;
using std::vector;
using std::unique_ptr;
using std::function;

#ifndef SCENE_H
#define SCENE_H

// Contains all the geometry, lights, etc for a scene.
class Scene {
public:
    const vector<Primitive*> primitives;
    const vector<PointLight*> lights;

    Scene(const vector<Primitive*> primitives, const vector<PointLight*> lights):
        primitives(primitives), lights(lights)
    {
    }

    // param ray:              A ray, in scene coordinates, check intersection with.
    // param is_excluded_prim: Returns whether to discount intersections with
    //                         the given primtive.
    // return:                 The closest intersection to the start of the ray,
    //                         or null if no intersection was found.
    unique_ptr<Intersection> closest_intersection(const Ray &ray, const function<bool(const Primitive*)> is_excluded_prim) const {
        float closest_distance = std::numeric_limits<float>::max();
        int closest_primitive_idx = -1;
        // The intersection point in the scene's coordinate system.
        vec4 intersection_pos;

        for (size_t i=0; i<this->primitives.size(); i++) {
            if (is_excluded_prim(this->primitives[i])) {
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

        return unique_ptr<Intersection>(new Intersection(intersection_pos, primitives[closest_primitive_idx]));
    }

    // param ray:           A ray, in scene coordinates, check intersection with.
    // param excluded_prim: The primitive to discount intersections with.
    //                      This can be useful to avoid self-intersection.
    // return:              The closest intersection to the start of the ray,
    //                      or null if no intersection was found.
    unique_ptr<Intersection> closest_intersection(const Ray &ray, const Primitive *excluded_prim = nullptr) const {
        auto is_excluded_prim = [&](const Primitive *prim) {
            return prim == excluded_prim;
        };
        return this->closest_intersection(ray, is_excluded_prim);
    }

    // param ray:              A ray, in scene coordinates, check intersection with.
    // param excluded_obj_tag: The object to discount intersections with.
    // return:                 The closest intersection to the start of the ray,
    //                         or null if no intersection was found.
    unique_ptr<Intersection> closest_intersection_excluding_obj(const Ray &ray, const int excluded_obj_tag = -1) const {
        auto is_excluded_prim = [&](const Primitive *prim) {
            return prim->obj_tag == excluded_obj_tag;
        };
        return this->closest_intersection(ray, is_excluded_prim);
    }


    // param ray:           A ray, in scene coordinates, check intersection with.
    // param excluded_prim: The primitive to discount intersections with.
    //                      This can be useful to avoid self-intersection.
    // return:              All intersections along a ray.
    vector<Intersection> all_intersections(const Ray &ray, const Primitive *excluded_prim = nullptr) const {
        vector<Intersection> intersections;

        for (size_t i=0; i<this->primitives.size(); i++) {
            if (this->primitives[i] == excluded_prim) {
                continue;
            }

            unique_ptr<vec4> intersection_pos = primitives[i]->intersection(ray);

            if (intersection_pos != nullptr) {
                intersections.push_back(Intersection(*intersection_pos, this->primitives[i]));
            }
        }

        return intersections;
    }
};

#endif // SCENE_H
