#include "primitive.h"

#ifndef TORUS_H
#define TORUS_H

// A 2D primitive for a disk with a hole in the middle.
class Disc: public Primitive {
public:
    // The radius within which the disc will not be drawn.
    const float inner_r;
    // The radius outside of which the disc will not be drawn.
    const float outer_r;
    // The direction the disc is facing.
    const vec4 normal_dir;
    // The center of the disc.
    const vec4 center;

    Disc(float inner_r, float outer_r, vec4 normal_dir, vec4 center, const Shader *shader):
        Primitive(shader, Disc::make_bounding_cube(center, outer_r)),
        inner_r(inner_r), outer_r(outer_r), normal_dir(glm::normalize(normal_dir)), center(center) {
    };

    // return: the position of the intersection with the primitive, or nullptr
    //		   if no intersection occurred.
    optional<vec4> intersection(const Ray &ray) const {
        // With help from the equations from the link below for a plane and disc.
        //  https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#eqn:vectray

        vec3 normal_3d = vec3(this->normal_dir);
        vec3 ray_dir_3d = vec3(ray.dir);

        vec3 ray_offset_3d = vec3(this->center - ray.start);
        float n_dot_offset = glm::dot(normal_3d, ray_offset_3d);
        float n_dot_d = glm::dot(normal_3d, ray_dir_3d);

        float t = n_dot_offset / n_dot_d;

        // The disc is behind the start of the ray.
        if (t < 0) {
            return nullopt;
        }

        vec4 intersection = ray.start + (t * ray.dir);
        vec4 intersection_offset = intersection - this->center;
        vec3 offset_3d = vec3(intersection_offset);

        // The squared distance from the center of the disc to the intersection
        // point.
        float sq_dist = glm::dot(offset_3d, offset_3d);

        // If the square distance is within the square radii then the
        // intersection occurred within the disc.
        if (inner_r * inner_r <= sq_dist && sq_dist <= outer_r * outer_r) {
            return intersection;
        }

        return nullopt;
    }

    // return: the normal to the primtive at the given point on the primitive.
    vec4 normal_at(vec4 point) const {
        return normal_dir;
    }

private:
    // return: the minimum and maximum corners of bounding box around the torus.
    static BoundingCube make_bounding_cube(vec4 center, float outer_r) {
        // Init 3D min an max.
        vec3 min_3d, max_3d;
        for (int i=0; i<3; i++) {
            min_3d[i] = center[i] - outer_r;
            max_3d[i] = center[i] + outer_r;
        }

        return BoundingCube(project_to_4D(min_3d), project_to_4D(max_3d));
    }
};

#endif // TORUS_H
