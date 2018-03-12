#include <glm/glm.hpp>
#include "../geometry/scene.h"
#include "../shaders/shader.h"
#include "camera.h"
#include "SDLauxiliary.h"
#include "omp.h"
#include "../geometry/random.h"
#include <optional>

using std::optional;

#ifndef RENDERER_H
#define RENDERER_H

// return: the color in the scene at the point where the ray intersects the scene.
vec3 colour_in_scene(Scene &scene, Ray &ray, const int num_shadow_rays) {
    optional<Intersection> i = scene.closest_intersection(ray);

    if (!i.has_value()) {
        return vec3(0, 0, 0);
    }

    vec3 acc_colour = vec3(0, 0, 0);
    // Colour is addative for all lights.
    for (const Light *light: scene.lights) {
        // Try casting to a specular light. If that does work, assume the
        // light is an ambient light.
        const SpecularLight* specular = dynamic_cast<const SpecularLight*>(light);

        if (specular != nullptr) {
            acc_colour += i->primitive->shader->shadowed_color(i->pos, i->primitive, ray, scene, *specular, num_shadow_rays);
        } else {
            acc_colour += i->primitive->shader->ambient_color(i->pos, i->primitive, *light);
        }
    }

    return acc_colour;
}

// return: a vector containing a single target (x, y). Useful for testing.
vector<vec2> single_target(int x, int y) {
    vector<vec2> targets;
    targets.push_back(vec2(x, y));
    return targets;
}

// return: a number of random points to sample for the pixel with center (x, y).
vector<vec2> random_screen_targets(int x, int y, int num_rays) {
    vector<vec2> targets;
    for (int i=0; i<num_rays; i++) {
        vec2 pixel_center = vec2((float)x, (float)y);
        // We sample an area around the pixel too, hence the 1.5
        vec2 target = random_in_box(pixel_center, 0.5, 0.5);
        targets.push_back(target);
    }
    return targets;
}

// return: a number of points in a grid to sample for the pixel with center (x, y).
vector<vec2> grid_primary_rays(int x, int y, int num_rays) {
    // num_rays = 4;
    //
    // float pixel_size = 5.0f; // Sample outside the pixel.
    // float ray_step = pixel_size / (float)num_rays;
    //
    // vector<vec2> targets;
    // for (int i=0; i<num_rays; i++) {
    //     for (int j=0; j<num_rays; j++) {
    //         float tx = i * ray_step - pixel_size / 2;
    //         float ty = j * ray_step - pixel_size / 2;
    //
            // targets.push_back(vec2(tx + x, ty + y));
    //     }
    // }

    vector<vec2> targets;
    float delta = 0.02f, step = 0.01f;
    for (float i=-delta; i<=delta; i+=step) {
        for (float j=-delta; j<=delta; j+=step) {
            targets.push_back(vec2(x + i, y + j));
        }
    }

    return targets;
}

// return: fires a number of primary rays randomly into the pixel, and computes
//         the mean color. Therefore performing anti-aliasing.
vec3 mean_pixel_color(Scene &scene, Camera &camera, screen *screen, const int x, const int y, int num_primary_rays, int num_shadow_rays) {
    //vector<vec2> targets = random_screen_targets(x, y, num_primary_rays);
    //vector<vec2> targets = grid_primary_rays(x, y, num_primary_rays);
    vector<vec2> targets = single_target(x, y);

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
