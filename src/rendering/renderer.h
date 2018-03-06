#include <glm/glm.hpp>
#include "../geometry/scene.h"
#include "../shaders/shader.h"
#include "camera.h"
#include "SDLauxiliary.h"
#include "omp.h"
#include "../geometry/random.h"

using std::unique_ptr;

#ifndef RENDERER_H
#define RENDERER_H

// return: the color in the scene at the point where the ray intersects the scene.
vec3 colour_in_scene(Scene &scene, Ray &ray, const int num_shadow_rays) {
    unique_ptr<Intersection> i = scene.closest_intersection(ray);

    if (!i) {
        return vec3(0, 0, 0);
    }

    vec3 acc_colour = vec3(0, 0, 0);
    // Colour is addative for all lights.
    for (const PointLight *light: scene.lights) {
        acc_colour += i->primitive->shader->shadowed_color(i->pos, i->primitive, ray, scene, *light, num_shadow_rays);
    }

    return acc_colour;
}

// return: a number of rays in the pixel with center at (x, y)
vector<vec2> random_screen_targets(int x, int y, int num_rays) {
    vector<vec2> targets;
    for (int i=0; i<num_rays; i++) {
        vec2 pixel_center = vec2((float)x, (float)y);
        // We sample an area around the pixel too, hence the 1.5
        vec2 target = random_in_box(pixel_center, 1.5, 1.5);
        targets.push_back(target);
    }
    return targets;
}

vector<Ray> grid_primary_rays(Camera &camera, int x, int y, screen *screen, int num_rays) {
    float ray_step = 1.0f / (float)num_rays;

    for (int i=0; i<num_rays; i++) {
        for (int j=0; j<num_rays; j++) {

        }
    }
}

// return: fires a number of primary rays randomly into the pixel, and computes
//         the mean color. Therefore performing anti-aliasing.
vec3 mean_pixel_color(Scene &scene, Camera &camera, screen *screen, const int x, const int y, int num_primary_rays, int num_shadow_rays) {
    //vector<Ray> rays = random_primary_rays(camera, x, y, screen, num_primary_rays);
    vector<vec2> targets = random_screen_targets(x, y, num_primary_rays);

    // Used to total up the color of all the pixels so it can be averaged.
    vec3 acc_color = vec3(0, 0, 0);
    for (vec2 target: targets) {
        Ray ray = camera.primary_ray(target.x, target.y, screen->width, screen->height);
        acc_color += colour_in_scene(scene, ray, num_shadow_rays);
    }
    return acc_color / (float)targets.size();
}

// effect: renders the scene to the screen buffer using the camera.
// param num_samples: the number of primary rays to average per pixel.
// param num_shadow_rays: the number of rays to shoot to the the sphere around the light.
void render(Scene &scene, Camera &camera, screen* screen, const int num_samples, const int num_shadow_rays) {
    #pragma omp parallel for
    for (int y=0; y<screen->height; y++) {
        for (int x=0; x<screen->width; x++) {
            vec3 color = mean_pixel_color(scene, camera, screen, x, y, num_samples, num_shadow_rays);
            PutPixelSDL(screen, x, y, color);
        }
    }
}

#endif // RENDERER_H
