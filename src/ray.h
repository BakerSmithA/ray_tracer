#include <glm/glm.hpp>

using glm::vec4;

#ifndef RAY_H
#define RAY_H

// return: the projection of vec into the line spanned by line_vec.
vec4 orthogonal_projection(vec4 vec, vec4 line_vec) {
    // For reference, see
    //  https://en.wikibooks.org/wiki/Linear_Algebra/Orthogonal_Projection_Onto_a_Line
    float x = glm::dot(vec, line_vec);
    float y = glm::dot(line_vec, line_vec);
    return x / y * line_vec;
}

// return: the length of the shortest vector connecting vec to the line spanned
//         by line_vec.
float orthogonal_dist(vec4 vec, vec4 line_vec) {
    vec4 proj = orthogonal_projection(vec, line_vec);
    // The vector connecting the line to vec.
    vec4 diff = vec - proj;
    return glm::length(diff);
}

#endif // RAY_H
