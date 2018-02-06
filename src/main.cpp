#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "models/TestModelH.h"
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include "mass.h"
#include "ray.h"

using namespace std;
using glm::vec3;
using glm::mat3;
using glm::vec4;
using glm::mat4;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false

typedef struct Intersection {
    // The 4D position of an intersection.
    vec4 position;
    // The distance from the camera center of projection to the intersection.
    float distance;
    // The the triangle that was intersected.
    Triangle *triangle;
    // The index of the triangle.
    int tri_index;

} Intersection;

// returns: a 4D point projected into 3D space.
vec3 project_4D(vec4 vec) {
    return vec3(vec.x, vec.y, vec.z);
}

// returns: the intersection point (t, u, v) between the ray (decribed the start
//          and direction vectors) and the triangle.
vec3 intersection(vec4 start, vec4 dir, Triangle tri) {
    // See 2.1: Triangles in 3D from lab 2.
    vec4 v0 = tri.v0;
    vec4 v1 = tri.v1;
    vec4 v2 = tri.v2;

    // See equations (4) and (5) from lab 2.
    vec3 e1 = project_4D(v1 - v0);
    vec3 e2 = project_4D(v2 - v0);

    // See 2.3: Intersection from lab2.
    // Intersecting the ray and the plane desribed by the triangle's basis
    // vectors e1 and e2.
    vec3 b = project_4D(start - v0);
    mat3 A(-project_4D(dir), e1, e2);
    return glm::inverse(A) * b;
}

// param x: the vector representing (t, u, v) to describe a point in the triangle's plane.
// returns: whether a point on the plane of the triangle lies **inside** the triangle.
bool is_inside_triangle(vec3 x) {
    float t = x.x;
    float u = x.y;
    float v = x.z;

    return 0 < t && 0 <= u && 0 <= v && u + v <= 1;
}

// param u: used to describe a point in the triangle's plane.
// param v: used to describe a point in the triangle's plane.
// param tri: the triangle in which the point lies.
// returns: a 4D point representing the vector from the camera to the point on the triangle.
vec4 convert_to_camera_coordinates(float u, float v, Triangle tri) {
    vec4 e1 = tri.v1 - tri.v0;
    vec4 e2 = tri.v2 - tri.v0;

    return tri.v0 + u * e1 + v * e2;
}

// param start: the start point of the 4D ray.
// param dir: the direction of the 4D ray.
// param triangles: all triangles in the scene.
// param closest_intersection: set to the intersection with the closest triangle to the camera.
// param camera_origin: the position of the camera. Used to calculate the distance from points to the camera.
// param exclude_tri_index: ignores this triangle, even if an intersection is
//                          found. This can be used to avoid self-occlusion.
// returns: whether an intersection point was found to triangles in the scene.
bool closest_intersection(vec4 start, vec4 dir, vector<Triangle> &triangles, Intersection &closest_intersection, int exclude_tri_index = -1) {
    closest_intersection.distance = std::numeric_limits<float>::max();
    bool found = false;

    for (int i=0; i<triangles.size(); i++) {
        vec3 x = intersection(start, dir, triangles[i]);

        if (exclude_tri_index != i && is_inside_triangle(x)) {
            vec4 intersection_point = convert_to_camera_coordinates(x.y, x.z, triangles[i]);
            float dist = glm::length(intersection_point - start);

            if (dist < closest_intersection.distance) {
                closest_intersection.position = intersection_point;
                closest_intersection.distance = dist;
                closest_intersection.triangle = &triangles[i];
                closest_intersection.tri_index = i;

                found = true;
            }
        }
    }

    return found;
}

// param intersection_pos: the 4D position of the intersection we want to whether the light can reach.
// param point_to_light: the vector from the intersection point to the light source.
// param exclude_tri_index: ignores this triangle, even if an intersection is
//                          found. This can be used to avoid self-occlusion.
// returns: whether the path of light from the light to the intersection is
//          occluded by another object.
bool is_occluded(vec4 start, vec4 dir, vector<Triangle> triangles, int exclude_tri_index) {
    float dist_to_light = glm::length(dir);

    Intersection i;
    bool found = closest_intersection(start, normalize(dir), triangles, i, exclude_tri_index);

    if (found) {
        // The distance from the start to the intersected point must be less
        // than the distance to the light source, otherwise the intersection
        // occurred 'after' the light.
        float intersection_dist = glm::length(start - i.position);
        return intersection_dist <= dist_to_light;
    }

    return false;
}

// param point_to_light: the vector from the intersection point to the light source.
// returns: the illumination at the given intersection point.
vec3 direct_light(vec4 shadow_ray, vec4 normal, vec4 light_pos, vec3 light_col) {
    // The distance from the light source to the intersection, i.e. the radius of the sphere.
    float r = glm::length(shadow_ray);

    // The proportion of light hitting the surface.
    float prop = glm::dot(glm::normalize(normal), glm::normalize(shadow_ray));

    // Because negative light is not allowed.
    float projection_factor = std::max(prop, 0.0f);

    // Because the power of the light hitting a point follows the inverse square law.
    float surface_area_at_radius_r = 4 * M_PI * r * r;
    vec3 power_per_surface_area_at_radius_r = light_col / surface_area_at_radius_r;

    return projection_factor * power_per_surface_area_at_radius_r;
}

// Models indirect and direct lighting.
vec3 indirect_light(Intersection &i, vec4 light_pos, vec3 light_col, vector<Triangle> &triangles) {
    // Vector from the intersection point to the light source.
    vec4 shadow_ray = light_pos - i.position;

    if (is_occluded(i.position, shadow_ray, triangles, i.tri_index)) {
        // Indirect light.
        return vec3(0.1, 0.1, 0.1);
    }

    return direct_light(shadow_ray, i.triangle->normal, light_pos, light_col);
}

// returns: a 4D direction vector from the camera's origin to a pixel.
vec4 primary_ray(int x_pixelCoordinate, int y_pixelCoordinate, int width, int height, float focal_length, vec4 camera_pos) {
    float x_cameraCoordinate = x_pixelCoordinate - width/2;
    float y_cameraCoordinate = y_pixelCoordinate - height/2;
    vec4 dir = vec4(x_cameraCoordinate - camera_pos.x, y_cameraCoordinate - camera_pos.y, focal_length - camera_pos.z, 1);
    return glm::normalize(dir);
}

/*Place your drawing here*/
void draw(screen* screen, vector<Triangle> triangles, float focal_length, vec4 camera_pos, vec4 light_pos, vec3 light_col) {
    /* Clear buffer */
    memset(screen->buffer, 0, screen->height*screen->width*sizeof(uint32_t));

    for (int y=0; y<SCREEN_HEIGHT; y++) {
        for (int x=0; x<SCREEN_WIDTH; x++) {
            vec4 dir = primary_ray(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, focal_length, camera_pos);

            Intersection intersection;
            bool found = closest_intersection(camera_pos, dir, triangles, intersection);

            if (found) {
                vec3 light_intensity = indirect_light(intersection, light_pos, light_col, triangles);
                vec3 colour = intersection.triangle->color * light_intensity;
                PutPixelSDL(screen, x, y, colour);
            }
        }
    }
}

/*Place updates of parameters here*/
void update(vec4& camera_pos, vec4& light_pos) {
    static int t = SDL_GetTicks();
    /* Compute frame time */
    int t2 = SDL_GetTicks();
    float dt = float(t2-t);
    t = t2;

    std::cout << "Render time: " << dt << " ms" << std::endl;

    //Translate Camera Position
    const uint8_t* scancodes = SDL_GetKeyboardState(NULL);
    if(scancodes[SDL_SCANCODE_DOWN]) camera_pos.z -= 0.01;
    else if(scancodes[SDL_SCANCODE_UP]) camera_pos.z += 0.01;
    else if(scancodes[SDL_SCANCODE_RIGHT]) camera_pos.x += 0.01;
    else if (scancodes[SDL_SCANCODE_LEFT]) camera_pos.x -= 0.01;

    //Translate Light Source
    else if(scancodes[SDL_SCANCODE_A]) light_pos.x -= 0.01;
    else if(scancodes[SDL_SCANCODE_D]) light_pos.x += 0.01;
    else if(scancodes[SDL_SCANCODE_Q]) light_pos.y -= 0.01;
    else if(scancodes[SDL_SCANCODE_E]) light_pos.y += 0.01;
    else if(scancodes[SDL_SCANCODE_S]) light_pos.z -= 0.01;
    else if(scancodes[SDL_SCANCODE_W]) light_pos.z += 0.01;
}

int main(int argc, const char* argv[]) {
    screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);

    vector<Triangle> triangles;
    LoadTestModel(triangles);

    float focal_length = SCREEN_WIDTH/2;
    vec4 camera_pos(0, 0, -2.25, 1);

    vec4 light_pos(0, -0.5, -0.7, 1.0);
    vec3 light_col = 18.0f * vec3(1, 1, 1);

    while(NoQuitMessageSDL()){
        update(camera_pos, light_pos);
        draw(screen, triangles, focal_length, camera_pos, light_pos, light_col);
        SDL_Renderframe(screen);
    }

    SDL_SaveImage(screen, "screenshot.bmp");

    KillSDL(screen);
    return 0;
}
