#pragma once

using ConvertDistToColor = function<vec3(float closest_dist,
                                         vec4 projection,
                                         vec4 diff,
                                         vec4 position,
                                         const Primitive *prim,
                                         const Ray &incoming,
                                         const Scene &scene,
                                         const Light &light,
                                         const int num_shadow_rays)>;

// Shader which can be customised to give different colours depending on the
// distance from the center of an object.
class DistFromCenter: public Shader {
public:
    ConvertDistToColor dist_to_color;

    DistFromCenter(ConvertDistToColor dist_to_color): dist_to_color(dist_to_color) {
    }

    // return: the color of the intersected surface. Takes occulsion of the
    //         light, by other objects, into account.
    vec3 shadowed_color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) const {
        if (!incoming.can_bounce()) {
            return vec3(0.0f);
        }

        // The center of the object to which primitive belongs.
        vec4 lens_center = prim->parent_obj->center();
        // Line from the ray start to the center of the object to which primitive belongs.
        vec4 line_to_center = incoming.start - lens_center;

        // A scalar multiple of ray, to the vector that is orthogonal to the center.
        vec4 projection = orthogonal_projection(line_to_center, incoming.dir);
        // The shortest vector from the ray to the center.
        vec4 diff = line_to_center - projection;
        // The length of the shortest distance from the ray to the center.
        float closest_dist = glm::length(vec3(diff));

        return this->dist_to_color(closest_dist, projection, diff, position, prim, incoming, scene, light, num_shadow_rays);
    }

    // return: a DistFromCenter shader which only uses the distance to generate
    //         a color.
    static DistFromCenter *dist(function<vec3(float)> dist_to_color) {
        auto f = [=](float closest_dist, vec4 projection, vec4 diff, vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const Light &light, const int num_shadow_rays) {
            return dist_to_color(closest_dist);
        };

        return new DistFromCenter(f);
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
};
