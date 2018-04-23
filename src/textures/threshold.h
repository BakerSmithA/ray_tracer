#include <glm/glm.hpp>

#ifndef THRESHOLD_2D_H
#define THRESHOLD_2D_H

// A texture node which thresholds the supplied texture if the euclidean
// distance of the vector represented by the RGB values is above the given
// threshold.
template<typename Vec>
class Threshold: public Texture<Vec> {
public:
    const Texture<Vec> *texture;
    const float threshold;

    Threshold(const Texture<Vec> *texture, float threshold):
        texture(texture), threshold(threshold) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the color of the the texture at the given position in the
    //         coordinate space of the object.
    vec3 color_at(Vec position) const {
        vec3 col = this->texture->color_at(position);
        return col.x < this->threshold ? vec3(0.0f) : vec3(1.0f);
    }

    // return: the alpha of the texture at the given position.
    // float alpha_at(vec2 position) const {
    //     float alpha = this->texture->alpha_at(position);
    //     return alpha < this->threshold ? 0.0f : 1.0f;
    // }
};

#endif // THRESHOLD_2D_H
