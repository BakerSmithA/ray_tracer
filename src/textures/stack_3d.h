#ifndef STACK_3D_H
#define STACK_3D_H

// A 3d texture made from stacking 2d textures. All the invidivual images are
// contained within one large image, the frames.
//
// Inspiration from:
//  See http://shaderbits.com/blog/authoring-pseudo-volume-textures
class Stack3d: public Texture3d {
public:
    // An image containing all the frames. Assumes a square image with the
    // same number of frames on the x- and y-axes.
    const Texture2d *frames;
    // The number of frames on the x- and y-axes.
    const int num_frames_per_side;
    const float inv_num_frames_per_side;
    // The total number of frames.
    const int num_frames;

    Stack3d(const Texture2d *frames, const int num_frames_per_side):
        frames(frames),
        num_frames_per_side(num_frames_per_side),
        inv_num_frames_per_side(1.0f/(float)num_frames_per_side),
        num_frames(num_frames_per_side * num_frames_per_side)
    {
    }

    Stack3d(const char *frames_image_name, const int num_frames_per_side):
        Stack3d(new File2d(frames_image_name), num_frames_per_side)
    {
    }

    // param position: position in the texture where each axis goes from 0-1.
    // return: the density of the the texture at the given position in the
    //         coordinate space of the object. The denser this is, the less
    //         light will be allowed to pass through.
    float density_at(vec4 pos) const {
        // Multiplied by the position to create an frame position.
        vec3 m = vec3(this->inv_num_frames_per_side, this->inv_num_frames_per_side, this->num_frames);
        // Slices the volume texture along the z-axis.
        vec3 slice_pos_side = vec3(pos) * m;
        // Linearly interpolate the z-axis to avoid slicing artefacts.
        return lerp_density_at(slice_pos_side.x, slice_pos_side.y, slice_pos_side.z);
    }

private:
    // return: the bilinearly interpolated color at the given position in
    //         image space, i.e. x and y go from 0 to width and height respectively.
    float lerp_density_at(float x, float y, float z) const {
        // Bound the indices to be inside the buffer.
        int min_z = min((int)floor(z), this->num_frames - 1);
        int max_z = min((int)ceil(z), this->num_frames - 1);

        // Need to be careful of division by zero, otherwise get black lines
        // appearing on texture. If the min and max are the same, mix the
        // samples in equal proportions.
        float inv_depth = (max_z - min_z) == 0.0f ? 0.5f : 1.0f / (max_z - min_z);

        // The color of the pixel at the same x and y, but at different depths.
        // This allows for interpolation along the depth axis.
        float density1 = this->density_pixel_at(x, y, min_z);
        float density2 = this->density_pixel_at(x, y, max_z);

        return glm::mix(density2, density1, (max_z - z) * inv_depth);
    }

    // return: the density of the pixel at the given x, y, frame_index in the
    //         space of the frames buffer. Therefore the x, y, and frame_index
    //         go from 0 to frames width, height, and frame count respectively.
    float density_pixel_at(float x, float y, int frame_index) const {
        // Convert the 1d frame index into a 2d index.
        int frame_index_x = frame_index % num_frames_per_side;
        int frame_index_y = frame_index * this->inv_num_frames_per_side;

        // The origin of the frame in the frames image.
        float frame_start_x = (float)frame_index_x * this->inv_num_frames_per_side;
        float frame_start_y = (float)frame_index_y * this->inv_num_frames_per_side;

        vec2 pos = vec2(x + frame_start_x, y + frame_start_y);
        return this->frames->color_at(pos).x;
    }
};

#endif // STACK_3D_H
