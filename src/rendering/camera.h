#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    vec4 pos;
    float focal_length;

    Camera(vec4 pos, float focal_length): pos(pos), focal_length(focal_length) {
    }

    Ray primary_ray(int pixel_x, int pixel_y, int screen_width, int screen_height) {
        float camera_x = pixel_x - screen_width / 2;
        float camera_y = pixel_y - screen_height / 2;

        vec4 dir = vec4(camera_x - this->pos.x, camera_y - this->pos.y, this->focal_length - this->pos.z, 1);

        return Ray(this->pos, dir);
    }
};

#endif // CAMERA_H
