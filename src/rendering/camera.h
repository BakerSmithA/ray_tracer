#ifndef CAMERA_H
#define CAMERA_H

class Camera {
private:
    vec4 pos;
    float yaw;

public:
    float focal_length;
    // The maximum number of bounces light rays from this camera can make.
    const int max_ray_bounces;

    Camera(vec4 pos, float focal_length, int max_ray_bounces):
        pos(pos), yaw(0.0f), focal_length(focal_length), max_ray_bounces(max_ray_bounces)
    {
    }

    Ray primary_ray(int pixel_x, int pixel_y, int screen_width, int screen_height) {
        float camera_x = pixel_x - screen_width / 2;
        float camera_y = pixel_y - screen_height / 2;

        vec4 dir = vec4(camera_x - this->pos.x, camera_y - this->pos.y, this->focal_length - this->pos.z, 1);
        vec4 rotated_dir = this->yaw_matrix() * dir;

        return Ray(this->pos, rotated_dir, this->max_ray_bounces);
    }

    // effect: moves the camera relative to the direction it is facing.
    void move(vec4 unit_direction, float distance) {
        vec4 rotated_dir = this->yaw_matrix() * unit_direction * distance;
        this->pos += rotated_dir;
    }

    // effect: moves the camera forwards relative to its direction.
    void move_forward(float distance) {
        vec4 vec = vec4(0.0f, 0.0f, 1.0f, 1.0f); // Move in the z-axis.
        this->move(vec, distance);
    }

    // effect: moves the camera downwards.
    void move_down(float distance) {
        vec4 vec = vec4(0.0f, 1.0f, 0.0f, 1.0f); // Move in the y-axis.
        this->move(vec, distance);
    }

    // effect: moves the camera right, relative to its direction.
    void move_right(float distance) {
        vec4 vec = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Move in the x-axis.
        this->move(vec, distance);
    }

    // effect: yaws the camera by the given amount.
    void turn(float delta_yaw) {
        this->yaw += delta_yaw;
    }

private:
    // return: the yaw matrix for the yaw of the camera.
    mat4 yaw_matrix() {
        vec4 a = vec4(cos(yaw), 0.0f, -sin(yaw), 0.0f);
        vec4 b = vec4(0.0f,     1.0f,      0.0f, 0.0f);
        vec4 c = vec4(sin(yaw), 0.0f,  cos(yaw), 0.0f);
        vec4 d = vec4(0.0f,     0.0f,      0.0f, 1.0f);

        return mat4(a, b, c, d);
    }
};

#endif // CAMERA_H
