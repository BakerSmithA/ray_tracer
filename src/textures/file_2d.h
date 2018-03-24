#include "texture_2d.h"

#ifndef FILE_2D_H
#define FILE_2D_H

class File2d: public Texture2d {
private:
    SDL_Surface *image;

public:
    const float width, height;

    File2d(const char *image_name):
        image(SDL_LoadBMP(image_name)), width(image->w), height(image->h) {
        if (image == NULL) {
            printf("Unable to load bitmap: %s\n", SDL_GetError());
            exit(1);
        }
    }

    ~File2d() {
        SDL_FreeSurface(this->image);
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(vec2 uv) const override {
        // TODO: Mix color of pixels.
        vec2 clamped = glm::clamp(uv, vec2(0, 0), vec2(1, 1));
        vec2 image_uv = vec2(clamped.x * this->image->w, clamped.y * this->image->h);
        return get_pixel(this->image, (int)image_uv.x, (int)image_uv.y);
    }
};

#endif // FILE_2D_H
