#include "texture_2d.h"

#ifndef FILE_2D_H
#define FILE_2D_H

// To avoid divisions when reading pixels as the RGB values from files are
// from 0-255, whereas they need to be from 0-1.
#define INV_255 (1.0f / 255.0f)

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
        Uint32 pixel = get_pixel_uint(x, y);

        uint8_t b = pixel & 0xFF;
        uint8_t g = (pixel >> 8) & 0xFF;
        uint8_t r = (pixel >> 16) & 0xFF;

        return vec3(r * INV_255, g * INV_255, b * INV_255);
    }

    // return: the width of the image buffer.
    int width() const {
        return this->image->w;
    }

    // return: the height of the image buffer.
    int height() const {
        return this->image->h;
    }

private:
    Uint32 get_pixel_uint(int x, int y) const {
        int bpp = this->image->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8 *p = (Uint8 *)this->image->pixels + y * this->image->pitch + x * bpp;

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
            /* shouldn't happen, but avoids warnings */
            return 0;
        }
    }
};

#endif // FILE_2D_H
