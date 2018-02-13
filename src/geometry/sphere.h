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

    Sphere(vec4 center, float radius, Shader* shader) 
        : Primitive(shader), center(center), radius(radius) {};

	virtual unique_ptr<vec4> intersection(const Ray &ray) const override {
        vec3 orig = vec3(ray.start); 
        vec3 dir = normalize(vec3(ray.dir));
        float radius2 = radius * radius;

        vec3 L = vec3(center) - orig;
        float tca = dot(L,dir);
        // if (tca < 0) return false;
        float d2 = dot(L,L) - tca * tca; 
        if (d2 > radius2) return nullptr; 
        float thc = sqrt(radius2 - d2); 
        float t0 = tca - thc; 
        float t1 = tca + thc; 

        if(thc < 0) { //cannot square root negative number (no intersection)
            printf("sqrt\n");
            return nullptr;
        }
        //Analytic solution
        else {
            float t0 = tca + thc;
            float t1 = tca - thc;
            
            if (t0 > t1) {
                std::swap(t0, t1); //pick the closer intersection
            }
 
            if (t0 < 0) { 
                t0 = t1; // if t0 is negative, let's use t1 instead 
                if (t0 < 0) {
                    printf("points outside scene\n");
                    return nullptr; // both t0 and t1 are negative 
                }
            } 
            float t = t0; 
            //printf("intersection found\n");

            //printf("t[%f] less than length(ray.dir)[%f]\n", t, length(ray.dir));
            return unique_ptr<vec4>(new vec4(project_to_4D(vec3(ray.start) + (t * dir))));
        }
    }

    virtual vec4 compute_normal(vec4 point) const override {
        return normalize(point - center);
    }
};

#endif // SPHERE_H


    //  //Geometric solution
    //     vec3 L = vec3(center - ray.start);
    //     vec3 dir = normalize(vec3(ray.dir));
    //     float tca = glm::dot(L, dir);
    //     float d2 = glm::dot(L, L) - tca * tca;
    //     float radius2 = radius * radius;
    //     float thc = radius2 - d2;