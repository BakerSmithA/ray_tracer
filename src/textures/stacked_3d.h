#include "texture_3d.h"
#include "file_2d.h"

#ifndef STACKED_3D_H
#define STACKED_3D_H

// A 3d texture made from stacking 2d textures.
class Stacked3d: public Texture3d {
public:
    // Stores the images which are stacked. All images are stored in the same
    // image, which is then split up.
    const File2d *frames;
    const int frames_per_side;
    const float frame_size;

    Stacked3d(File2d *frames, int frames_per_side):
        frames(frames), frames_per_side(frames_per_side), frame_size(frames->width / frames_per_side) {
    }

    Stacked3d(const char *frames_file_name, int frames_per_side):
        Stacked3d(new File2d(frames_file_name), frames_per_side) {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    float density_at(vec4 position) const override {
        vec2 p = vec2(position) / 6.0f;
        return this->frames->color_at(p).x;
    }
};

#endif // STACKED_3D_H
