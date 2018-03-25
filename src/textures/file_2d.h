#include "texture_2d.h"

#ifndef FILE_2D_H
#define FILE_2D_H

class File2d: public BilinearTexture2d {
private:
    SDL_Surface *image;

public:
    File2d(const char *image_name):
        image(SDL_LoadBMP(image_name)) {
        if (image == NULL) {
            printf("Unable to load bitmap: %s\n", SDL_GetError());
            exit(1);
        }
    }

    ~File2d() {
        SDL_FreeSurface(this->image);
    }

    // return: the color of the pixel at the given x, y in the space of the
    //         image buffer. Therefore the x and y go from 0 to image width
    //         and height respectively.
    vec3 pixel_at(int x, int y) const {
        return get_pixel(this->image, x, y);
    }

    // return: the width of the image buffer.
    int width() const {
        return this->image->w;
    }

    // return: the height of the image buffer.
    int height() const {
        return this->image->h;
    }
};

#endif // FILE_2D_H
