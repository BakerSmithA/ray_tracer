#pragma once

template<typename Vec>
class Texture {
    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    virtual vec3 color_at(Vec position) const = 0;
};

template<typename Vec>
class LerpedTexture: Texture<Vec> {
    vec3 color_at(Vec position) const {
        Vec image_uv = position * this->buffer_size();
        return this->bilinear_color_at_pixel(image_uv.x, image_uv.y);
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
    vec3 lerped_color_at(vec3 pos) const {
        // With help from:
        //  http://supercomputingblog.com/graphics/coding-bilinear-interpolation/
        Vec max_buffer_idx = this->buffer_size() - Vec(1.0f);
        // Clamp to ensure the min and max are inside the texture buffer.
        Vec min = glm::clamp(glm::floor(pos), Vec(0.0f), max_buffer_idx);
        Vec max = glm::clamp(glm::ceil(pos), Vec(0.0f), max_buffer_idx);
        Vec diff = max - min;
        Vec inv = diff == Vec(0.0f) ? Vec(0.5f) : 1.0f / diff;
        Vec prop = (max - pos) * inv;

        return this->lerp(min, max, prop);
    }
};

// Specialised implementation for 2d textures.
template<>
vec3 LerpedTexture<vec2>::lerp(vec2 pixel_min_pos, vec2 pixel_max_pos, vec2 prop) const {
    return vec3(1);
};

// Specialised implementation for 3d textures.
template<>
vec3 LerpedTexture<vec3>::lerp(vec3 pixel_min_pos, vec3 pixel_max_pos, vec3 prop) const {
    return vec3(0);
};
