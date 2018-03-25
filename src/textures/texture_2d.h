#include <glm/glm.hpp>

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

// Interface for a 2D texture filling a space from 0-1 in each axis.
class Texture2d {
public:
    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    virtual vec3 color_at(vec2 position) const = 0;
};

// Interface for a 2D texture created from a buffer of pixels, which can
// be interpolated to generate a smooth texture.
class BilinearTexture2d: public Texture2d {
public:
    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(vec2 position) const {
        vec2 clamped = glm::clamp(vec2(0,0), vec2(1,1), position);
        vec2 image_uv = clamped * vec2(this->width(), this->height());
        return this->bilinear_color_at_pixel(image_uv.x, image_uv.y);
    }

    // return: the color of the pixel at the given x, y in the space of the
    //         image buffer. Therefore the x and y go from 0 to image width
    //         and height respectively.
    virtual vec3 pixel_at(int x, int y) const = 0;

    // return: the width of the image buffer.
    virtual int width() const = 0;

    // return: the height of the image buffer.
    virtual int height() const = 0;

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
        vec3 col11 = this->pixel_at(min_x, min_y);
        vec3 col12 = this->pixel_at(min_x, max_y);
        vec3 col21 = this->pixel_at(max_x, min_y);
        vec3 col22 = this->pixel_at(max_x, max_y);

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

#endif // TEXTURE_2D_H
