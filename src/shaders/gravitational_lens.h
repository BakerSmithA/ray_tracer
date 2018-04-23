#pragma once

#include "shader.h"

// A shader which deflects light rays according to an inverse square
// relationship with distance to the center of the object.
class GravitationalLens: public Shader {
public:
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

        return vec3(closest_dist);
    }

    // return: 100% transparency, as this shader only deflects light, it does
    //         not block it.
    float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return 0.0f;
    }

private:
    // return: the deflection angle of a ray which passed closest_distance
    //         distance to the center of the lens.
    float deflection_angle(float closest_distance, float max_dist) const {
        float prop = closest_distance / max_dist;
        return 1.0f / prop * prop;
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
};
