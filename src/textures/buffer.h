#ifndef BUFFER_2D_H
#define BUFFER_2D_H

// A texture created by bilinearly interpolating over a buffer of colors.
// It is the responsibility of the creator of the buffer to free the buffer.
template<typename Vec>
class Buffer: public LerpedTexture<Vec> {
public:
    const Vec buff_size;
    const vec3 *buffer;

    // param buffer_size: width, height, depth, etc of the buffer.
    // param seed: the seed used when generating random numbers.
    Buffer(Vec buffer_size, vec3 *buffer):
        buff_size(buffer_size), buffer(buffer){
    }

    // return: the color of the pixel at the given x, y in the space of the
    //         image buffer. Therefore the x and y go from 0 to image width
    //         and height respectively.
    vec3 pixel_at(Vec pos) const {
        return vec3(1.0f, 0.0f, 0.0f);
    }

    // return: the size of the buffer holding the pixels.
    Vec buffer_size() const {
        return this->buff_size;
    }
};

// Specialised pixel at for 2d textures.
template<>
vec3 Buffer<vec2>::pixel_at(vec2 pos) const {
    int i = (int)(pos.y * this->buff_size.x + pos.x);
    return this->buffer[i];
}

template<>
vec3 Buffer<vec3>::pixel_at(vec3 pos) const {
    int i = (int)(pos.x + buff_size.x * (pos.y + buff_size.y * pos.z));
    return this->buffer[i];
}

#endif // BUFFER_2D_H
