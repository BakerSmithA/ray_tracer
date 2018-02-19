#include <glm/glm.hpp>
#include <SDL.h>
#include "../rendering/SDLauxiliary.h"
#include "shader.h"

using glm::vec2;

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture: public Shader {
private:
    SDL_Surface *image = NULL;

public:
    Texture(const char *image_name): image(SDL_LoadBMP(image_name)) {
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

        int u = proj_x * image->w; // from 0 to image width
        int v = proj_y * image->h; // from 0 to image height

        return get_pixel(this->image, u, v);
    }
};

#endif // TEXTURE_H
