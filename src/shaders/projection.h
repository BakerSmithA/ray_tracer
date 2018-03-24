#include <glm/glm.hpp>
#include <SDL.h>
#include "../rendering/SDLauxiliary.h"
#include "shader.h"
#include "../textures/file_2d.h"
#include <math.h>
#include <functional>

using glm::vec2;
using glm::normalize;
using glm::length;
using std::function;

#ifndef PROJECTION_TEXTURE_H
#define PROJECTION_TEXTURE_H

// The axis to 'shine' a texture along in to generate a planar projection.
enum PlanarProjectionDirection {
    planar_x, planar_y, planar_z
};

// return: the point (in object space) projected to uv texture coordinates.
vec2 planar_projected(vec4 object_space_point, PlanarProjectionDirection dir) {
    // Uses the planar mapping described in:
    //  http://cs-people.bu.edu/sbargal/Fall%202016/lecture_notes/Dec_5_Advanced_Projection_Mapping_And_Ray_Tracing.pdf

    float u = 0, v = 0;

    switch (dir) {
        // Projecting the texture along the x-axis.
        case planar_x:
            u = object_space_point.z;
            v = object_space_point.y;
            break;
        // Projecting the texture along the y-axis.
        case planar_y:
            u = object_space_point.x;
            v = object_space_point.z;
            break;
        // Projecting the texture along the z-axis.
        case planar_z:
            u = object_space_point.x;
            v = object_space_point.y;
            break;
    }

    return vec2(u, v);
}

// return: the point (in object space) projected onto uv texture coordinates.
vec2 spherical_projected(vec4 object_space_point) {
    // Uses the spherical mapping described in:
    //  http://cs-people.bu.edu/sbargal/Fall%202016/lecture_notes/Dec_5_Advanced_Projection_Mapping_And_Ray_Tracing.pdf

    // The object space point is in the range 0-1, therefore the center of the
    // sphere is at (0.5, 0.5, 0.5).
    vec3 p = vec3(object_space_point) - vec3(0.5, 0.5, 0.5);

    // Conversion to angles was done using the formula here:
    //  https://www.opengl.org/discussion_boards/showthread.php/159883-converting-a-3D-vector-into-three-euler-angles
    float radius = length(p);

    // Angles describing the point on the circle.
    // Add pi to make in the range 0-2pi
    float angle1 = atan2(p.z, p.x) + (M_PI);
    // Multiply by 2 to make in range 0-2pi
    float angle2 = acos(p.y / radius) * 2;

    // Change into the range 0-1.
    float u = angle1 / (2 * M_PI);
    float v = angle2 / (2 * M_PI);

    // 1-v to correctly inverert the y-axis
    return vec2(u, 1-v);
}

class Projection: public ShadowedShader {
public:
    // The texture projected onto the surface.
    const Texture2d *texture;

    // Takes a point in 4d object coordinates to a point 2d uv coordinate.
    const function<vec2(vec4)> project_to_uv;

    Projection(Texture2d *texture, const function<vec2(vec4)> project_to_uv):
        texture(texture), project_to_uv(project_to_uv)
    {
    }

    // return: the color of the intersected surface, illuminated by a specular
    //         light, i.e. a directional light, point light, etc.
    vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // Convert the position u,v coordinate (i.e. in the object's coordinate
        // space for planar mapping).
        vec4 proj = prim->parent_obj->converted_world_to_obj(position);
        // uv is in the range 0-1.
        vec2 uv = this->project_to_uv(proj);
        // Converts uv to be in image coordinates.
        return this->texture->color_at(uv);
    }

    /*
     ** Convenience functions for creating shaders with different
     ** projection methods.
     */

    // return: a texture shader which maps the texture using planar mapping
    //         in the given direction.
    static Projection *planar(const char *image_name, PlanarProjectionDirection dir) {
        const auto project_to_uv = [=](vec4 object_coordinate) {
            return planar_projected(object_coordinate, dir);
        };

        Texture2d *texture = new File2d(image_name);
        return new Projection(texture, project_to_uv);
    }

    // return: a texture shader which maps the texture using a spherical projection.
    static Projection *spherical(const char *image_name) {
        Texture2d *texture = new File2d(image_name);
        return new Projection(texture, spherical_projected);
    }
};

#endif // PROJECTION_TEXTURE_H
