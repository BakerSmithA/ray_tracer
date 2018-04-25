#pragma once

#include <glm/glm.hpp>
#include <tuple>
#include "texture.h"

using std::tuple;
using std::get;

// 1D ramp texture which blends multiple colors together.
class Ramp: public Texture<float> {
private:
    // Colors and the position from 0-1 where they are at full intensity.
    vector<tuple<vec3, float> > colors;

public:

    Ramp(vector<tuple<vec3, float> > colors): colors(colors) {
        std::sort(begin(colors), end(colors), [](tuple<vec3, float> const &t1, tuple<vec3, float> const &t2) {
            return get<1>(t1) < get<1>(t2);
        });
    }

    vec3 color_at(float position) const {
        int before_idx = this->col_idx_before(position);
        int after_idx = before_idx + 1;

        tuple<vec3, float> before = this->colors[before_idx];
        tuple<vec3, float> after = this->colors[after_idx];

        float diff = get<1>(after) - get<1>(before);
        float inv_diff = diff == 0.0f ? 0.5 : 1.0f / diff;
        float prop = (position - get<1>(before)) * inv_diff;

        return glm::mix(get<0>(before), get<0>(after), prop);
    }

private:
    int col_idx_before(float position) const {
        // -1 because the after index is calculated as this index +1.
        for (size_t i=0; i<colors.size()-1; i++) {
            if (get<1>(colors[i]) > position) {
                return i;
            }
        }

        std::runtime_error("Ramp: position outside of color range");
    }
};
