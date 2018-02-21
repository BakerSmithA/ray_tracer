#include <glm/glm.hpp>
#include "primitive.h"
#include "projection.h"
#include <math.h>

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::dot;

#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Primitive {
public:
    vec4 center;
    float radius;

    Sphere(vec4 center, float radius, Shader* shader, const int obj_tag):
        Primitive(shader, Sphere::make_bounding_cube(center, radius), obj_tag),
        center(center),
        radius(radius)
    {
    }

	virtual unique_ptr<vec4> intersection(const Ray &ray) const override {
        vec3 orig = vec3(ray.start);
        vec3 dir = normalize(vec3(ray.dir));
        float radius2 = radius * radius;

        //Computing inside of sqrt
        vec3 L = vec3(center) - orig;
        float tca = dot(L,dir);
        float d2 = dot(L,L) - tca * tca;
        if (d2 > radius2) { //cannot compute sqrt of negative number
            return nullptr;
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
                return nullptr; // both t0 and t1 are negative
            }
        }
        float t = t0;

        return unique_ptr<vec4>(new vec4(project_to_4D(vec3(ray.start) + (t * dir))));
    }

    virtual vec4 compute_normal(vec4 point) const override {
        return normalize(point - center);
    }

private:
    // return: the minimum and maximum corners of bounding box around the
    //         sphere.
    static BoundingCube make_bounding_cube(vec4 center, float radius) {
        //throw std::runtime_error("Not implemented yet - circle bounding cube");
        return BoundingCube(vec4(), vec4());
    }
};

#endif // SPHERE_H
