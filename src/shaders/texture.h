#include <glm/glm.hpp>
#include <SDL.h>
#include "../rendering/SDLauxiliary.h"
#include "shader.h"
#include <math.h>
#include "../debugging.h"

using glm::vec2;
using glm::normalize;
using glm::length;

#ifndef TEXTURE_H
#define TEXTURE_H

// The axis to 'shine' a texture along in to generate a planar projection.
enum PlanarProjectionDirection {
    planar_x, planar_y, planar_z
};

// return: the point (in object space) projected to uv texture coordinates.
vec2 planar_projected(vec4 object_space_point, PlanarProjectionDirection dir) {
    // Uses the planar mapping described in:
    //  http://cs-people.bu.edu/sbargal/Fall%202016/lecture_notes/Dec_5_Advanced_Texture_Mapping_And_Ray_Tracing.pdf

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
    //  http://cs-people.bu.edu/sbargal/Fall%202016/lecture_notes/Dec_5_Advanced_Texture_Mapping_And_Ray_Tracing.pdf

    // The object space point is in the range 0-1, therefore the center of the
    // sphere is at (0.5, 0.5, 0.5).
    vec3 p = vec3(object_space_point);

    // Conversion to angles was done using the formula here:
    //  https://www.opengl.org/discussion_boards/showthread.php/159883-converting-a-3D-vector-into-three-euler-angles
    float radius = length(p);
    float u = atan2(p.y, p.x);
    float v = atan2(p.z, radius);

    return vec2(u, v);
}

class Texture: public Shader {
private:
    SDL_Surface *image = NULL;

public:
    const PlanarProjectionDirection projection_dir;

    Texture(const char *image_name, PlanarProjectionDirection dir):
        image(SDL_LoadBMP(image_name)), projection_dir(dir)
    {
        if (image == NULL) {
            printf("Unable to load bitmap: %s\n", SDL_GetError());
            exit(1);
        }
    }

    ~Texture() {
        SDL_FreeSurface(this->image);
    }

    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const {
        // Convert the position u,v coordinate (i.e. in the object's coordinate
        // space for planar mapping).
        vec4 proj = prim->parent_obj->converted_world_to_obj(position);
        // uv is in the range 0-1.
        vec2 uv = planar_projected(proj, this->projection_dir);
        // image_uv is in the range 0-image size.
        vec2 image_uv = vec2(uv.x * this->image->w, uv.y * this->image->h);

        return get_pixel(this->image, (int)image_uv.x, (int)image_uv.y);
    }
};

#endif // TEXTURE_H
