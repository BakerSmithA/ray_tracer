#ifndef BUFFER_2D_H
#define BUFFER_2D_H

// A texture created by bilinearly interpolating over a buffer of colors.
// It is the responsibility of the creator of the buffer to free the buffer.
class Buffer2d: public BilinearTexture2d {
public:
    const vec3 *buffer;
    const int buff_width, buff_height;

    // param width: the width of the generated texture.
    // param height: the height of the generated texture.
    // param seed: the seed used when generating random numbers.
    Buffer2d(int width, int height, vec3 *buffer):
        buffer(buffer), buff_width(width), buff_height(height) {
    }

    // return: the color of the pixel at the given x, y in the space of the
    //         image buffer. Therefore the x and y go from 0 to image width
    //         and height respectively.
    vec3 pixel_at(int x, int y) const {
        return this->buffer[y * this->buff_width + x];
    }

    // return: the width of the image buffer.
    int width() const {
        return this->buff_width;
    }

    // return: the height of the image buffer.
    int height() const {
        return this->buff_height;
    }
};

#endif // BUFFER_2D_H
