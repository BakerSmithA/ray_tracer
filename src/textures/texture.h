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

    // return: the number of dimensions of the texture.
    int dims() const {
        return Vec().length();
    }
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
    virtual vec3 lerp(Vec pixel_min_pos, Vec pixel_max_pos, Vec prop) const {
        throw std::runtime_error("Non implemented lerp for texture");
    }

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
        // Vec diff = max - min;
        // Vec inv = diff == Vec(0.0f) ? Vec(0.5f) : 1.0f / diff;
        // Vec prop = (max - pos) * diff;
        Vec prop = (max - pos);
        return this->lerp(min, max, prop);
    }
};

// Specialise linear interpolation for 2d textures.
template<>
vec3 LerpedTexture<vec2>::lerp(vec2 min, vec2 max, vec2 prop) const {
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

// Specialise linear interpolation for 3d textures.
template<>
vec3 LerpedTexture<vec3>::lerp(vec3 min, vec3 max, vec3 prop) const {
    vec3 col111 = this->pixel_at(vec3(min.x, min.y, min.z));
    vec3 col121 = this->pixel_at(vec3(min.x, max.y, min.z));
    vec3 col211 = this->pixel_at(vec3(max.x, min.y, min.z));
    vec3 col221 = this->pixel_at(vec3(max.x, max.y, min.z));

    vec3 col112 = this->pixel_at(vec3(min.x, min.y, max.z));
    vec3 col122 = this->pixel_at(vec3(min.x, max.y, max.z));
    vec3 col212 = this->pixel_at(vec3(max.x, min.y, max.z));
    vec3 col222 = this->pixel_at(vec3(max.x, max.y, max.z));

    // Interpolate along the x-axes.
    vec3 x11 = fast_lerp(col211, col111, prop.x);
    vec3 x21 = fast_lerp(col221, col121, prop.x);
    vec3 x12 = fast_lerp(col212, col112, prop.x);
    vec3 x22 = fast_lerp(col222, col122, prop.x);

    // Interpolate along the y-axes.
    vec3 y1 = fast_lerp(x21, x11, prop.y);
    vec3 y2 = fast_lerp(x22, x12, prop.y);

    // Interpolate along the z-axes.
    return fast_lerp(y2, y1, prop.z);
}
