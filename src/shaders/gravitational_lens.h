#pragma once

#include "shader.h"

// A shader which deflects light rays according to an inverse square
// relationship with distance to the center of the object.
class GravitationalLens: public Shader {
public:
    // The maximum distance from the center of the object that a ray can
    // intersect the the object.
    float max_dist;

    GravitationalLens(float max_dist): max_dist(max_dist) {
    }

    // return: the color of the intersected surface. Takes occulsion of the
    //         light, by other objects, into account.
    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // The center of the object to which primitive belongs.
        vec4 lens_center = prim->parent_obj->center();
        // Line from the ray start to the center of the object to which primitive belongs.
        vec4 line_to_center = incoming.start - lens_center;

        // A scalar multiple of ray, to the vector that is orthogonal to the mass.
        vec4 projection = orthogonal_projection(line_to_center, incoming.dir);
        // The shortest vector from the ray to the mass.
        vec4 diff = line_to_center - projection;
        // The length of the shortest distance from the ray to the mass.
        float closest_dist = glm::length(vec3(diff));
        // The angle to deflect the ray towards the center of the lens.
        float angle = this->deflection_angle(closest_dist);

        return vec3(angle);
    }

    // return: 100% transparency, as this shader only deflects light, it does
    //         not block it.
    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return 0.0f;
    }

private:
    // return: the deflection angle of a ray which passed closest_distance
    //         distance to the center of the lens.
    float deflection_angle(float closest_distance) const {
        float prop = closest_distance / this->max_dist;
        return prop * prop;
    }

    // return: the projection of vec into the line spanned by line_vec. The
    //         returned vector is a scalar multiple of line_vec.
    vec4 orthogonal_projection(vec4 vec, vec4 line_vec) const {
        // For reference, see
        //  https://en.wikibooks.org/wiki/Linear_Algebra/Orthogonal_Projection_Onto_a_Line
        float x = dot(vec, line_vec);
        float y = dot(line_vec, line_vec);
        return x / y * line_vec;
    }

    // return: the vector ray after deflecting it by angle in the plane defined by
    //         vectors a and b.
    // vec3 deflected(vec3 dor, float angle, vec3 a, vec3 b) {
    //     // Uses Rodrigues' rotation formula
    //     //  https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    //
    //     // A vector perdendicular to a and b.
    //     vec3 perp = cross(a, b);
    //
    //     // The axis about which vector v is rotated by angle.
    //     vec3 axis = normalize(perp);
    //
    //     vec3 x = ray * cos(angle);
    //     vec3 y = cross(axis, ray) * sin(angle);
    //     vec3 z = axis * dot(axis, ray) * (1 - cos(angle));
    //
    //     return x + y + z;
    // }
};
