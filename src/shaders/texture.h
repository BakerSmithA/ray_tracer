#include <glm/glm.hpp>
#include <SDL.h>
#include "shader.h"

using glm::vec2;

#ifndef TEXTURE_H
#define TEXTURE_H

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

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

    // ~Texture() {
    //     SDL_FreeSurface(this->image);
    // }

    // return: the color of the intersected surface, as illuminated by a specific light.
    virtual vec3 color(const vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const {
        // Convert the position u,v coordinate (i.e. in the object's coordinate
        // space for planar mapping).
        float proj_x = (position.x + 1) / 2; // from 0 to 1
        float proj_y = (position.y + 1) / 2; // from 0 to 1

        int u = proj_x * image->w; // from 0 to image width
        int v = proj_y * image->h; // from 0 to image height

        Uint32 pixel = getpixel(image, u, v);

        uint8_t b = pixel & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t r = (pixel >> 16) & 0xFF;

        return vec3((float)r / 255, (float)g / 255, (float)b / 255);
    }
};

#endif // TEXTURE_H
