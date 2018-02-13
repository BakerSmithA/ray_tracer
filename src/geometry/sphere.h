#include <glm/glm.hpp>
#include "primitive.h"
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

    Sphere(vec4 center, float radius, Shader shader) 
        : center(center), radius(radius), Primitive(shader) {};

	virtual unique_ptr<vec3> intersection(const Ray &ray) const override {
        vec3 sminc = ray.start - center;
        vec3 d = normalize(ray.dir);
        
        float a = glm::dot(d, sminc);
        float b = glm::dot(sminc, sminc);
        float c = a * a - b - radius * radius;
        if(c < 0) return nullptr;
        else {
            float t = - a + sqrt(c);
            if(t < 0) return nullptr;
            else {
                return ray.start + t * d;
            }
        }
    }

    virtual vec4 compute_normal(vec4 point) const override {
        return normalize(point - center);
    }
};

#endif // SPHERE_H
