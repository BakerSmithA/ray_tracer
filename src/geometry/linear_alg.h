#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec3;
using glm::mat3;

#ifndef LINEAR_ALG_H
#define LINEAR_ALG_H

// return: the determinant of the the matrix A
__attribute__((always_inline))
float determinant(const float source[16]) {
    // Column vec 0, i.e. A[0]
    float a = source[0];
    float b = source[1];
    float c = source[2];

    // Column vec 1, i.e. A[1]
    float d = source[3];
    float e = source[4];
    float f = source[5];

    // Column vec 1, i.e. A[2]
    float g = source[6];
    float h = source[7];
    float i = source[8];

    return a * (e*i - f*h)
         - b * (d*i - f*g)
         + c * (d*h - e*g);
}

// return: the determinant of the the matrix [ b, A[1], A[2] ]
__attribute__((always_inline))
float determinant0(const float source[16], const float source_b[3]) {
    // Column vec 0, i.e. b
    float a = source_b[0];
    float b = source_b[1];
    float c = source_b[2];

    // Column vec 1, i.e. A[1]
    float d = source[3];
    float e = source[4];
    float f = source[5];

    // Column vec 1, i.e. A[2]
    float g = source[6];
    float h = source[7];
    float i = source[8];

    return a * (e*i - f*h)
         - b * (d*i - f*g)
         + c * (d*h - e*g);
}

// return: the determinant of the the matrix [ A[0], b, A[2] ]
__attribute__((always_inline))
float determinant1(const float source[16], const float source_b[3]) {
    // Column vec 0, i.e. A[0]
    float a = source[0];
    float b = source[1];
    float c = source[2];

    // Column vec 1, i.e. b
    float d = source_b[0];
    float e = source_b[1];
    float f = source_b[2];

    // Column vec 2, i.e. A[2]
    float g = source[6];
    float h = source[7];
    float i = source[8];

    return a * (e*i - f*h)
         - b * (d*i - f*g)
         + c * (d*h - e*g);
}

// return: the determinant of the the matrix [ A[0], A[1], b ]
__attribute__((always_inline))
float determinant2(const float source[16], const float source_b[3]) {
    // Column vec 0, i.e. A[0]
    float a = source[0];
    float b = source[1];
    float c = source[2];

    // Column vec 1, i.e. A[1]
    float d = source[3];
    float e = source[4];
    float f = source[5];

    // Column vec 2, i.e. b
    float g = source_b[0];
    float h = source_b[1];
    float i = source_b[2];

    return a * (e*i - f*h)
         - b * (d*i - f*g)
         + c * (d*h - e*g);
}

// return: the solution Ax=b using Cramer's rule, described
//		   https://rosettacode.org/wiki/Cramer%27s_rule
vec3 solve_linear(mat3 A, vec3 b) {
    const float *source_A = (const float*)glm::value_ptr(A);
    const float *source_b = (const float*)glm::value_ptr(b);

    float inv_det_A = 1.0f / determinant(source_A);

    float x1 = determinant0(source_A, source_b) * inv_det_A;
    float x2 = determinant1(source_A, source_b) * inv_det_A;
    float x3 = determinant2(source_A, source_b) * inv_det_A;

    return vec3(x1, x2, x3);
}

// return: the dot product of vectors x and y.
float dot(const float x[3], const float y[3]) {
    return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

// return: the result of linearly interpolating using ratio defined by t.
__attribute__((always_inline))
float fast_lerp(float a, float b, float t) {
    return a * (1 - t) + b * t;
}

// return: the result of linearly interpolating all three axes using the
//         ratio defined by t.
vec3 fast_lerp(vec3 a, vec3 b, float t) {
    const float *source_a = (const float*)glm::value_ptr(a);
    const float *source_b = (const float*)glm::value_ptr(b);

    float lerp_a = fast_lerp(source_a[0], source_b[0], t);
    float lerp_b = fast_lerp(source_a[1], source_b[1], t);
    float lerp_c = fast_lerp(source_a[2], source_b[2], t);

    return vec3(lerp_a, lerp_b, lerp_c);
}

// return: the vector ray after deflecting it by angle in the plane defined by
//         vectors a and b.
vec3 deflected(vec3 incoming_dir, float angle, vec3 a, vec3 b) {
    // Uses Rodrigues' rotation formula
    //  https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

    // A vector perdendicular to a and b.
    vec3 perp = glm::cross(a, b);

    // The axis about which vector v is rotated by angle.
    vec3 axis = glm::normalize(perp);

    vec3 x = incoming_dir * (float)cos(angle);
    vec3 y = glm::cross(axis, incoming_dir) * (float)sin(angle);
    vec3 z = axis * glm::dot(axis, incoming_dir) * (1.0f - (float)cos(angle));

    return x + y + z;
}

#endif // LINEAR_ALG_H
