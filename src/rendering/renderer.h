#include <glm/glm.hpp>
#include "../geometry/scene.h"
#include "../shaders/shader.h"
#include "camera.h"
#include "SDLauxiliary.h"
#include "omp.h"

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

// effect: renders the scene to the screen buffer using the camera.
void render(Scene &scene, Camera &camera, screen* screen, const int num_shadow_rays) {
    #pragma omp parallel for
    for (int y=0; y<screen->height; y++) {
        for (int x=0; x<screen->width; x++) {
            Ray ray = camera.primary_ray(x, y, screen->width, screen->height);
            vec3 color = colour_in_scene(scene, ray, num_shadow_rays);
            PutPixelSDL(screen, x, y, color);
        }
    }
}

#endif // RENDERER_H
