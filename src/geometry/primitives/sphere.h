#pragma once

#include <glm/glm.hpp>
#include "primitive.h"
#include "../projection.h"
#include "../linear_alg.h"
#include <math.h>

using glm::vec3;
using glm::vec4;
using glm::normalize;
using std::optional;
using std::nullopt;

class Sphere : public Primitive {
public:
    vec4 center;
    float radius;

    Sphere(vec4 center, float radius, const Shader* shader):
        Primitive(shader, Sphere::make_bounding_cube(center, radius)),
        center(center),
        radius(radius)
    {
    }

	virtual optional<vec4> intersection(const Ray &ray) const override {
        vec3 orig = vec3(ray.start);
        vec3 dir = vec3(ray.normalized_dir);
        float radius2 = radius * radius;

        //Computing inside of sqrt
        vec3 L = vec3(center) - orig;

        const float *source_L = (const float*)glm::value_ptr(L);
        const float *source_dir = (const float*)glm::value_ptr(dir);

        float tca = dot(source_L, source_dir);
        float d2 = dot(source_L, source_L) - tca * tca;
        if (d2 > radius2) { //cannot compute sqrt of negative number
            return nullopt;
        }

        //Computing intersection points
        float thc = sqrt(radius2 - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        //Selecting closest intersecting point not beyond the boundary of the cornels box
        if (t0 > t1) {
            std::swap(t0, t1); //pick the closer intersection
        }

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) {
                //printf("points outside scene\n");
                return nullopt; // both t0 and t1 are negative
            }
        }
        float t = t0;

        return project_to_4D(vec3(ray.start) + (t * dir));
    }

    virtual vec4 normal_at(vec4 point) const override {
        return normalize(point - center);
    }

private:
    // return: the minimum and maximum corners of bounding box around the
    //         sphere.
    static BoundingCube make_bounding_cube(vec4 center, float radius) {
        // Init 3D min an max.
        vec3 min_3d, max_3d;
        for (int i=0; i<3; i++) {
            min_3d[i] = center[i] - radius;
            max_3d[i] = center[i] + radius;
        }

        return BoundingCube(project_to_4D(min_3d), project_to_4D(max_3d));
    }
};
