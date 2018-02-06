#include <glm/glm.hpp>
#include "models/TestModelH.h"

using glm::vec3;
using glm::vec4;
using glm::dot;
using glm::cross;
using glm::normalize;
using glm::cos;
using glm::sin;

using namespace std;

#ifndef RAY_H
#define RAY_H

// return: the projection of vec into the line spanned by line_vec. The
//         returned vector is a scalar multiple of line_vec.
vec4 orthogonal_projection(vec4 vec, vec4 line_vec) {
    // For reference, see
    //  https://en.wikibooks.org/wiki/Linear_Algebra/Orthogonal_Projection_Onto_a_Line
    float x = dot(vec, line_vec);
    float y = dot(line_vec, line_vec);
    return x / y * line_vec;
}

// return: the vector ray after deflecting it by angle in the plane defined by
//         vectors a and b.
vec3 deflected_3d(vec3 ray, float angle, vec3 a, vec3 b) {
    // Uses Rodrigues' rotation formula
    //  https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

    // A vector perdendicular to a and b.
    vec3 perp = cross(a, b);

    // The axis about which vector v is rotated by angle.
    vec3 axis = normalize(perp);

    vec3 x = ray * cos(angle);
    vec3 y = cross(axis, ray) * sin(angle);
    vec3 z = axis * dot(axis, ray) * (1 - cos(angle));

    return x + y + z;
}

// return: the 4D vector ray after deflecting it by angle in the 3D plane
//         defined by 4D vectors a and b.
vec4 deflected(vec4 ray, float angle, vec4 a, vec4 b) {
    vec3 ray_rot = deflected_3d(vec3(ray), angle, vec3(a), vec3(b));
    return vec4(ray_rot.x, ray_rot.y, ray_rot.z, 1.0);
}

#endif // RAY_H
