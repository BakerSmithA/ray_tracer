#pragma once

#include "../geometry/scene.h"
#include "../lights/light.h"

// return: the mean transparency from the intersection position to the
//         random points in the sphere of the light source.
float mean_random_transparency(vec4 pos, const Primitive *prim, const Scene &scene, const Light &light, const int num_shadow_rays);

// Used to model different types of surfaces, e.g. matte, glossy, etc.
class Shader {
public:
    virtual ~Shader() {
    }

    // param position: the position of the intersection of the prim with the shadow ray.
    // param prim: the primitive to calculate the transparency of.
    // param shadow_ray: the shadow ray from the original object the shadow is
    //                   being tested for, to the light source.
    // return: the proportion by which light is let through the
    //         material. E.g. a value of 1 is totally transparent, and a value
    //         of 0 is totally opaque.
    virtual float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        return 0.0f;
    }

    // return: the color of the intersected surface. Takes occulsion of the
    //         light, by other objects, into account.
    virtual vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const = 0;
};

// By subclassing this shader, a the materical can have its shadowed color
// automatically computed according to whether other objects are occluding the
// light. Also takes into account whether the light source can cast shadows.
class ShadowedShader: public Shader {
public:
    // return: the unshadowed color of the object. This is used to compute the
    //         shadowed color.
    virtual vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const = 0;

    // return: the color of the intersected surface. Takes occulsion of the
    //         light, by other objects, into account.
    virtual vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        // How much the light ray penetrates to the light source.
        // This is higher if it travels through transparent objects.
        float acc_transparency = mean_random_transparency(position, prim, scene, light, num_shadow_rays);

        const vec3 col = this->color(position, prim, incoming, scene, light, num_shadow_rays);
        return acc_transparency * col;
    }
};

// return: the how much a light ray penetrates from an intersection to
//         the light source.
float shadow_ray_transparency(vec4 pos, const Primitive *prim, const Scene &scene, const Ray &shadow_ray) {
    const float shadow_len = length(shadow_ray.dir);

    // The opacity of all objects on the way to the light determines
    // how much light gets through to this object, ignoring the primitive.
    vector<Intersection> all_intersections = scene.all_intersections(shadow_ray, prim);
    // The multiplication of all transparencies of all objects (before
    // the light) along the ray.
    float acc_mult_transparency = 1.0f;

    for (size_t i=0; i<all_intersections.size() && acc_mult_transparency >= 0.001; i++) {
        // If the intersection occurs between the light and this object.
        if (length(all_intersections[i].pos - shadow_ray.start) < shadow_len) {
            const Primitive *prim = all_intersections[i].primitive;
            acc_mult_transparency *= prim->shader->transparency(all_intersections[i].pos, prim, shadow_ray, scene);
        }
    }

    return acc_mult_transparency;
}

// return: the mean transparency from the intersection position to the
//         random points in the sphere of the light source. Accounts for if
//         the light cannot cast shadows.
float mean_random_transparency(vec4 pos, const Primitive *prim, const Scene &scene, const Light &light, const int num_shadow_rays) {
    if (num_shadow_rays == 0) {
        return 1.0f;
    }

    vector<Ray> shadow_rays = light.random_shadow_rays_from(pos, num_shadow_rays);

    // If the light does not support shadows, assume all the light reached
    // the surface.
    if (shadow_rays.size() == 0) {
        return 1.0;
    }

    // The addition of all transparencies of the different shadow rays.
    float acc_add_transparency = 0.0f;

    for (Ray shadow_ray: shadow_rays) {
        acc_add_transparency += shadow_ray_transparency(pos, prim, scene, shadow_ray);
    }

    // Calculate the mean by dividing by the number of rays.
    return acc_add_transparency / shadow_rays.size();
}
