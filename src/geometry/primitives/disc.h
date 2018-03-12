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
        Primitive(shader, Disc::make_bounding_cube()),
        inner_r(inner_r), outer_r(outer_r), normal_dir(glm::normalize(normal_dir)), center(center) {
    };

    // return: the position of the intersection with the primitive, or nullptr
    //		   if no intersection occurred.
    optional<vec4> intersection(const Ray &ray) const {
        // With help from the equations from the link below for a plane and disc.
        //  https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#eqn:vectray

        vec3 normal_3d = vec3(this->normal_dir);
        vec3 ray_dir_3d = vec3(ray.dir);

        vec3 offset_3d = vec3(this->center - ray.start);
        float n_dot_offset = glm::dot(normal_3d, offset_3d);
        float n_dot_d = glm::dot(normal_3d, ray_dir_3d);

        float t = n_dot_offset / n_dot_d;

        // The disc is behind the start of the ray.
        if (t < 0) {
            return nullopt;
        }

        return ray.start + (t * ray.dir);
    }

    // return: the normal to the primtive at the given point on the primitive.
    vec4 normal_at(vec4 point) const {
        return normal_dir;
    }

private:
    // return: whether the intersection point is within the bounds of the disc.
    bool is_inside(float t) const {
        return 0.0f < t;
    }

    // return: the minimum and maximum corners of bounding box around the torus.
    static BoundingCube make_bounding_cube() {
        // TODO
        return BoundingCube(vec4(), vec4());
    }
};

#endif // TORUS_H
