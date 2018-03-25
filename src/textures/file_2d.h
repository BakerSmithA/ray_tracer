#include "texture_2d.h"

#ifndef FILE_2D_H
#define FILE_2D_H

class File2d: public Texture2d {
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

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(vec2 uv) const override {
        vec2 image_uv = uv * vec2(this->image->w, this->image->h);
        return this->bilinear_color_at_pixel(image_uv.x, image_uv.y);
    }

private:
    // return: the bilinearly interpolated color at the given position in
    //         image space, i.e. x and y go from 0 to width and height respectively.
    vec3 bilinear_color_at_pixel(float x, float y) const {
        // Uses the formula described:
        //  http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
        float min_x = floor(x), min_y = floor(y);
        float max_x = ceil(x),  max_y = ceil(y);

        // Need to be careful of division by zero, otherwise get black lines
        // appearing on texture.
        float inv_width = (max_x - min_x) == 0.0f ? 0.5f : 1.0f / (max_x - min_x);
        float inv_height = (max_y - min_y) == 0.0f ? 0.5f : 1.0f / (max_y - min_y);

        // The colors of the pixels around location we want to sample.
        vec3 col11 = get_pixel(this->image, min_x, min_y);
        vec3 col12 = get_pixel(this->image, min_x, max_y);
        vec3 col21 = get_pixel(this->image, max_x, min_y);
        vec3 col22 = get_pixel(this->image, max_x, max_y);

        // Interpolate the color of the lower pixels along the x-axis.
        float x_prop = (max_x - x) * inv_width;
        vec3 r1 = glm::mix(col21, col11, x_prop);
        // Interpolate the color of the upper pixels along the x-axis.
        vec3 r2 = glm::mix(col22, col12, x_prop);
        // Interpolate along the y-axis.
        vec3 q = glm::mix(r2, r1, (max_y - y) * inv_height);

        return q;
    }
};

#endif // FILE_2D_H
