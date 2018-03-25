#include <glm/glm.hpp>
#include <limits.h>
#include <vector>
#include <functional>
#include <optional>
#include "intersection.h"
#include "object.h"
#include "projection.h"
#include "../lights/light.h"

using glm::length;
using std::vector;
using std::function;
using std::optional;
using std::nullopt;

#ifndef SCENE_H
#define SCENE_H

// Contains all the geometry, lights, etc for a scene.
//
// WARNING: when the scene is destroyed the objects, and lights, pointed to by
// the scene will also be destroyed.
class Scene {
public:
    // The number of objects in the scene.
    const int num_objects;
    // An array containing the objects in the scene.
    const Object **objects;
    // An array of the lights in the scene.
    const vector<Light*> lights;

    Scene(const int num_objects, const Object **objects, const vector<Light*> lights):
        num_objects(num_objects), objects(objects), lights(lights)
    {
    }

    ~Scene() {
        for (size_t i=0; i<lights.size(); i++) {
            delete lights[i];
        }

        for (int i=0; i<num_objects; i++) {
            delete objects[i];
        }
        delete[] objects;
    }

    // param ray:              A ray, in scene coordinates, check intersection with.
    // param is_excluded_prim: Returns whether to discount intersections with
    //                         the given primtive.
    // return:                 The closest intersection to the start of the ray,
    //                         or nothing if no intersection was found.
    optional<Intersection> closest_intersection(const Ray &ray, const function<bool(const Primitive*)> is_excluded_prim) const {
        float closest_distance = std::numeric_limits<float>::max();
        int closest_obj_idx = -1;
        int closest_primitive_idx = -1;
        // The intersection point in the scene's coordinate system.
        vec4 intersection_pos;

        for (int j=0; j<this->num_objects; j++) {
            const Object *object = this->objects[j];

            // Ignore objects for which the ray does not intersect the bounding
            // box of the object.
            if (!object->bounding_cube.does_intersect_ray(ray)) {
                continue;
            }

            Primitive **primitives = object->primitives;

            for (int i=0; i<object->num_prims; i++) {
                if (is_excluded_prim(primitives[i])) {
                    continue;
                }

                // The intersection in the scene coordinate system.
                optional<vec4> intersection = primitives[i]->intersection(ray);

                if (!intersection.has_value()) {
                    continue;
                }

                // The distance from the point in scene coordinates to the ray in
                // scene coordinates.
                float dist = length(*intersection - ray.start);

                if (dist < closest_distance) {
                    closest_distance = dist;
                    closest_obj_idx = j;
                    closest_primitive_idx = i;
                    intersection_pos = *intersection;
                }
            }
        }

        if (closest_primitive_idx == -1) {
            return nullopt;
        }

        Primitive *prim = this->objects[closest_obj_idx]->primitives[closest_primitive_idx];
        return Intersection(intersection_pos, prim);
    }

    // param ray:           A ray, in scene coordinates, check intersection with.
    // param excluded_prim: The primitive to discount intersections with.
    //                      This can be useful to avoid self-intersection.
    // return:              The closest intersection to the start of the ray,
    //                      or null if no intersection was found.
    optional<Intersection> closest_intersection(const Ray &ray, const Primitive *excluded_prim = nullptr) const {
        auto is_excluded_prim = [&](const Primitive *prim) {
            return prim == excluded_prim;
        };
        return this->closest_intersection(ray, is_excluded_prim);
    }

    // param ray:              A ray, in scene coordinates, check intersection with.
    // param excluded_obj_tag: The object to discount intersections with.
    // return:                 The closest intersection to the start of the ray,
    //                         or null if no intersection was found.
    optional<Intersection> closest_intersection_excluding_obj(const Ray &ray, const Object *excluded_obj) const {
        auto is_excluded_prim = [&](const Primitive *prim) {
            return prim->parent_obj == excluded_obj;
        };
        return this->closest_intersection(ray, is_excluded_prim);
    }


    // param ray:           A ray, in scene coordinates, check intersection with.
    // param excluded_prim: The primitive to discount intersections with.
    //                      This can be useful to avoid self-intersection.
    // return:              All intersections along a ray.
    vector<Intersection> all_intersections(const Ray &ray, const Primitive *excluded_prim = nullptr) const {
        vector<Intersection> intersections;

        for (int j=0; j<this->num_objects; j++) {
            Primitive **primitives = this->objects[j]->primitives;

            for (int i=0; i<this->objects[j]->num_prims; i++) {
                if (primitives[i] == excluded_prim) {
                    continue;
                }

                optional<vec4> intersection_pos = primitives[i]->intersection(ray);

                if (intersection_pos.has_value()) {
                    intersections.push_back(Intersection(*intersection_pos, primitives[i]));
                }
            }
        }

        return intersections;
    }
};

#endif // SCENE_H
