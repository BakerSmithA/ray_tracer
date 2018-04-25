#pragma once

#include "../textures/texture.h"
#include <tuple>

using std::tuple;
using std::get;

using ConvertDistToColor = function<vec3(float closest_dist, vec4 projection, vec4 diff, vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays)>;
using ConvertDistToAlpha = function<float(float closest_dist, vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene)>;

// Shader which can be customised to give different colours depending on the
// distance from the center of an object.
class DistFromCenter: public Shader {
public:
    ConvertDistToColor dist_to_color;
    ConvertDistToAlpha dist_to_alpha;
    float max_dist;

    DistFromCenter(ConvertDistToColor dist_to_color, ConvertDistToAlpha dist_to_alpha, float max_dist):
        dist_to_color(dist_to_color), dist_to_alpha(dist_to_alpha), max_dist(max_dist) {
    }

    DistFromCenter(ConvertDistToColor dist_to_color, float alpha, float max_dist):
        DistFromCenter(dist_to_color, DistFromCenter::const_dist_to_alpha(alpha), max_dist) {
    }

    // return: the color of the intersected surface. Takes occulsion of the
    //         light, by other objects, into account.
    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        if (!incoming.can_bounce()) {
            return vec3(0.0f);
        }

        tuple<float, vec4, vec4> dist_data = this->shortest_dist(prim, incoming);
        return this->dist_to_color(get<0>(dist_data), get<1>(dist_data), get<2>(dist_data), position, prim, incoming, scene, light, num_shadow_rays);
    }

    // param position: the position of the intersection of the prim with the shadow ray.
    // param prim: the primitive to calculate the transparency of.
    // param shadow_ray: the shadow ray from the original object the shadow is
    //                   being tested for, to the light source.
    // return: the proportion by which light is let through the
    //         material. E.g. a value of 1 is totally transparent, and a value
    //         of 0 is totally opaque.
    virtual float transparency(vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) const {
        tuple<float, vec4, vec4> dist_data = this->shortest_dist(prim, shadow_ray);
        return this->dist_to_alpha(get<0>(dist_data), position, prim, shadow_ray, scene);
    }

    // return: a DistFromCenter shader which only uses the distance to generate
    //         a color.
    static DistFromCenter *dist(function<vec3(float)> dist_to_color, function<float(float)> dist_to_alpha, float max_dist) {
        auto col = [=](float closest_dist, vec4 projection, vec4 diff, vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) {
            return dist_to_color(closest_dist);
        };

        auto alpha = [=](float closest_dist, vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) {
            return dist_to_alpha(closest_dist);
        };

        return new DistFromCenter(col, alpha, max_dist);
    }

    // return: a DistFromCenter shader which uses a 1d texture to decide the color.
    static DistFromCenter *textured(Texture<float> *color, Texture<float> *alpha, float max_dist) {
        auto dist_to_color = [=](float dist) {
            return color->color_at(dist);
        };

        auto dist_to_alpha = [=](float dist) {
            return color->color_at(dist).x;
        };

        return DistFromCenter::dist(dist_to_color, dist_to_alpha, max_dist);
    }

private:
    // return: the projection of vec into the line spanned by line_vec. The
    //         returned vector is a scalar multiple of line_vec.
    vec4 orthogonal_projection(vec4 vec, vec4 line_vec) const {
        // For reference, see
        //  https://en.wikibooks.org/wiki/Linear_Algebra/Orthogonal_Projection_Onto_a_Line
        float x = dot(vec, line_vec);
        float y = dot(line_vec, line_vec);
        return x / y * line_vec;
    }

    tuple<float, vec4, vec4> shortest_dist(const Primitive *prim, const Ray &incoming) const {
        // The center of the object to which primitive belongs.
        vec4 lens_center = prim->parent_obj->center();
        // Line from the ray start to the center of the object to which primitive belongs.
        vec4 line_to_center = incoming.start - lens_center;

        // A scalar multiple of ray, to the vector that is orthogonal to the center.
        vec4 projection = orthogonal_projection(line_to_center, incoming.dir);
        // The shortest vector from the ray to the center.
        vec4 diff = line_to_center - projection;
        // The length of the shortest distance from the ray to the center.
        float closest_dist = glm::length(vec3(diff)) / max_dist;

        return { closest_dist, projection, diff };
    }

    static ConvertDistToAlpha const_dist_to_alpha(float alpha) {
        auto dist_to_alpha = [=](float closest_dist, vec4 position, const Primitive *prim, const Ray &shadow_ray, const Scene &scene) {
            return alpha;
        };
        return dist_to_alpha;
    }
};
