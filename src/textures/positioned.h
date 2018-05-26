#pragma once

// A texture node which positions the texture in UV space.
template<typename Vec>
class Positioned: public Texture<Vec> {
public:
    const Texture<Vec> *texture;
    // Where the texture starts and ends in UV space.
    const Vec start, end;
    const Vec inv_size;
    // Used if the texture does not cover all UV space.
    const vec3 background;

    Positioned(const Texture<Vec> *texture, const Vec start, const Vec end, const vec3 background):
        texture(texture), start(start), end(end), inv_size(1.0f / (end - start)), background(background) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(Vec position) const {
        if (glm::all(glm::lessThanEqual(this->start, position))
         && glm::all(glm::lessThanEqual(position, this->end))) {
            Vec p = (position - this->start) * this->inv_size;
            return this->texture->color_at(p);
        }
        return this->background;
    }
};
