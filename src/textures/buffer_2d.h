#ifndef BUFFER_2D_H
#define BUFFER_2D_H

// A texture created by bilinearly interpolating over a buffer of colors.
// It is the responsibility of the creator of the buffer to free the buffer.
class Buffer2d: public LerpedTexture2d {
public:
    const vec3 *buffer;
    const int buff_width, buff_height;

    // param width: the width of the generated texture.
    // param height: the height of the generated texture.
    // param seed: the seed used when generating random numbers.
    Buffer2d(int width, int height, vec3 *buffer, bool use_red_as_alpha = false):
        // BilinearTexture2d(use_red_as_alpha), buffer(buffer), buff_width(width), buff_height(height) {
        buffer(buffer), buff_width(width), buff_height(height) {
    }

    // return: the color of the pixel at the given x, y in the space of the
    //         image buffer. Therefore the x and y go from 0 to image width
    //         and height respectively.
    vec3 pixel_at(vec2 pos) const {
        return this->buffer[(int)pos.y * this->buff_width + (int)pos.x];
    }

    // return: the size of the buffer holding the pixels.
    vec2 buffer_size() const {
        return vec2(this->buff_width, this->buff_height);
    }
};

#endif // BUFFER_2D_H
