#include "texture_3d.h"
#include "file_2d.h"

#ifndef STACKED_3D_H
#define STACKED_3D_H

// A 3d texture made from stacking 2d textures.
// Inspiration from:
//  See http://shaderbits.com/blog/authoring-pseudo-volume-textures
class Stacked3d: public Texture3d {
public:
    // Stores the images which are stacked. All images are stored in the same
    // image, which is then split up.
    const Texture2d *frames;
    const int frames_per_side;

    Stacked3d(Texture2d *frames, int frames_per_side):
        frames(frames), frames_per_side(frames_per_side) {
    }

    Stacked3d(const char *frames_file_name, int frames_per_side):
        Stacked3d(new File2d(frames_file_name), frames_per_side) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    float density_at(vec4 position) const override {
        float density_top = this->sample_density_top(position);
        float density_side = this->sample_density_side(position);

        return glm::mix(density_top, density_side, 0.8);
    }

private:
    // return: the density of the texture as viewed from above, perpendicular
    //         to the side. This is done by taking samples of single frames.
    //         (See http://shaderbits.com/blog/authoring-pseudo-volume-textures)
    float sample_density_top(vec4 position) const {
        // The pixel position in the frame we want to sample.
        vec2 sub_frame = vec2(position.x, position.z);
        // The origin of the frame we want to sample.
        int frame_index = (int)round(position.y * frames_per_side * frames_per_side);
        int frame_row = frame_index / frames_per_side;
        int frame_col = frame_index % frames_per_side;
        vec2 frame = vec2(frame_row, frame_col);

        vec2 uv = (sub_frame + frame) / (float)frames_per_side;

        // Only sample one channel since we only want density.
        return this->frames->color_at(uv).x;
    }

    // return: the density of the texture as viewed from the side, perpendicular
    //         to the top. This is done by composing rows of multiple frames.
    //         (See http://shaderbits.com/blog/authoring-pseudo-volume-textures)
    float sample_density_side(vec4 position) const {
        // The pixel position in the frame we want to sample.
        vec2 sub_frame = vec2(position.x, position.y);
        // The origin of the frame we want to sample.
        int frame_index = (int)round(position.z * frames_per_side * frames_per_side);
        int frame_row = frame_index / frames_per_side;
        int frame_col = frame_index % frames_per_side;
        vec2 frame = vec2(frame_row, frame_col);

        vec2 uv = (sub_frame + frame) / (float)frames_per_side;

        // Only sample one channel since we only want density.
        return this->frames->color_at(uv).x;
    }
};

// float m = 1.0f/(float)frames_per_side;
// vec2 pos = vec2(position.y, position.z);
// pos *= m;
//
// // Indexing each frames downwards row by row.
// float frame_index = (int)round(position.z * frames_per_side * frames_per_side);
// float frame_row = frame_index / frames_per_side;
// //int frame_col = frame_index % frames_per_side;
//
// // Index into the frames.
// pos += vec2(5 * m, 5 * m);
//
// return this->frames->color_at(vec2(pos)).x;

#endif // STACKED_3D_H
