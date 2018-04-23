#pragma once

template<typename Vec>
class Texture {
public:
    ~Texture() {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    virtual vec3 color_at(Vec position) const = 0;
};

template<typename Vec>
class LerpedTexture: public Texture<Vec> {
public:
    vec3 color_at(Vec position) const {
        Vec image_uv = position * this->buffer_size();
        return this->lerped_color_at(image_uv);
    }

    // return: the pixel value at the given position.
    virtual vec3 pixel_at(Vec position) const = 0;

    // return: the size of the buffer holding the texture in each axis.
    virtual Vec buffer_size() const = 0;

    // param prop: the proportion to mix the pixel min and pixel max in each axis.
    // return: linearly interpolated color in all axes between the min and max.
    virtual vec3 lerp(Vec pixel_min_pos, Vec pixel_max_pos, Vec prop) const = 0;

private:
    // return: the linearly interpolated color at the given position in
    //         image space, e.g. for a 2d texture the x and y go from 0 to
    //         width and height respectively.
    vec3 lerped_color_at(Vec pos) const {
        // With help from:
        //  http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
        Vec max_buffer_idx = this->buffer_size() - Vec(1.0f);
        // Clamp to ensure the min and max are inside the texture buffer.
        Vec min = glm::clamp(glm::floor(pos), Vec(0.0f), max_buffer_idx);
        Vec max = glm::clamp(glm::ceil(pos), Vec(0.0f), max_buffer_idx);
        Vec prop = (max - pos);
        return this->lerp(min, max, prop);
    }
};

class LerpedTexture2d: public LerpedTexture<vec2> {
    // param prop: the proportion to mix the pixel min and pixel max in each axis.
    // return: linearly interpolated color in all axes between the min and max.
    vec3 lerp(vec2 min, vec2 max, vec2 prop) const {
        vec3 col11 = this->pixel_at(vec2(min.x, min.y));
        vec3 col12 = this->pixel_at(vec2(min.x, max.y));
        vec3 col21 = this->pixel_at(vec2(max.x, min.y));
        vec3 col22 = this->pixel_at(vec2(max.x, max.y));

        // Interpolate the color of the lower pixels along the x-axis.
        vec3 r1 = fast_lerp(col21, col11, prop.x);
        // Interpolate the color of the upper pixels along the x-axis.
        vec3 r2 = fast_lerp(col22, col12, prop.x);
        // Interpolate along the y-axis.
        return fast_lerp(r2, r1, prop.y);
    }
};
