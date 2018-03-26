#ifndef STACK_3D_H
#define STACK_3D_H

// A 3d texture made from stacking 2d textures. All the invidivual images are
// contained within one large image, the frames.
//
// Inspiration from:
//  See http://shaderbits.com/blog/authoring-pseudo-volume-textures
class Stack3d: public Texture3d {
private:
    // An image containing all the frames. Assumes a square image with the
    // same number of frames on the x- and y-axes.
    SDL_Surface *frames;

public:
    // The number of frames on the x- and y-axes.
    const int num_frames_per_side;
    // The side length of a frame.
    const int frame_size;

    Stack3d(const char *frames_image_name, const int num_frames_per_side):
        frames(SDL_LoadBMP(frames_image_name)),
        num_frames_per_side(num_frames_per_side),
        frame_size(this->frames->w / num_frames_per_side)
    {
        if (frames == NULL) {
            printf("Unable to load bitmap: %s\n", SDL_GetError());
            exit(1);
        }
    }

    ~Stack3d() {
        SDL_FreeSurface(this->frames);
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    float density_at(vec4 position) const {
        // Slices the volume texture along the z-axis.
        vec3 slice_side = vec3(position) * vec3(this->frame_size, this->frame_size, this->num_frames_per_side);
        return density_pixel_at(slice_side.x, slice_side.y, slice_side.z);
    }

    // return: the density of the pixel at the given x, y, frame_index in the
    //         space of the frames buffer. Therefore the x, y, and frame_index
    //         go from 0 to frames width, height, and frame count respectively.
    float density_pixel_at(int x, int y, int frame_index) const {
        // Calculate the position at which the frame (according to frame_index)
        // starts.
        int frame_start_x = frame_index / num_frames_per_side;
        int frame_start_y = frame_index % num_frames_per_side;

        return get_pixel(this->frames, x + frame_start_x * frame_size, y + frame_start_y * frame_size).x;
    }
};

#endif // STACK_3D_H
