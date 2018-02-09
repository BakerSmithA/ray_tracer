#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    vec4 position;
    float focal_length;

    Camera(vec4 position, float focal_length): position(position), focal_length(focal_length) {
    }

    Ray primary_ray(int pixel_x, int pixel_y, int screen_width, int screen_height) {
        float camera_x = pixel_x - screen_width / 2;
        float camera_y = pixel_y - screen_height / 2;

        vec4 dir = vec4(camera_x - this->position.x, camera_y - this->position.y, this->focal_length - this->position.z, 1);

        return Ray(this->position, dir);
    }
};

#endif // CAMERA_H
