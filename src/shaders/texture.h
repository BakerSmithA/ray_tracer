#include <glm/glm.hpp>
#include <SDL.h>
#include "../rendering/SDLauxiliary.h"
#include "shader.h"

using glm::vec2;

#ifndef TEXTURE_H
#define TEXTURE_H

// The axis to 'shine' a texture along in to generate a planar projection.
enum PlanarProjectionDirection {
    planar_x, planar_y, planar_z
};

// return: the point (in object space) projected to u, v coordinates.
vec2 planar_projected(vec4 object_space_point, PlanarProjectionDirection dir, int img_width, int img_height) {
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

    return vec2(u * img_width, v * img_height);
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
        float proj_x = (position.x + 1) / 2; // from 0 to 1
        float proj_y = (position.y + 1) / 2; // from 0 to 1
        float proj_z = (position.z + 1) / 2; // from 0 to 1

        vec4 proj = vec4(proj_x, proj_y, proj_z, 1.0f);

        float u = proj_x * image->w; // from 0 to image width
        float v = proj_y * image->h; // from 0 to image height

        vec2 uv = planar_projected(proj, this->projection_dir, this->image->w, this->image->h);

        return get_pixel(this->image, (int)uv.x, (int)uv.y);
    }
};

#endif // TEXTURE_H
