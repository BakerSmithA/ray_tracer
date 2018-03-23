#include "light.h"

#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class DirectionalLight: public Light {
public:
    // The direction of the parallel light rays.
    const vec4 normalised_dir;
    // The length of the shadow rays from an intersection to the directional light.
    // This is used since the light does not have a position.
    const float shadow_ray_len;
    // Used to create soft shadows. There is no physical radius of the light,
    // however there is a radius to the sphere to which shadow rays rays will
    // be created.
    const float radius;

    DirectionalLight(vec3 color, vec4 dir, float shadow_ray_len, float radius):
        Light(color), normalised_dir(glm::normalize(dir)), shadow_ray_len(shadow_ray_len), radius(radius) {
    }

    // The intensity of a directional light depends only on the angle of the
    // light to the surface normal. It is assumed the light is infinitely far
    // away, hence why the light rays are parallel.
    //
    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of each color channel of the light at the given
    //         position in the scene.
    vec3 intensity(vec4 point) const {
        return this->color;
    }

    // param position: the position on the object to get the factor of.
    // param surface_normal: the normal of the illuminated surface.
    // return: the proportion of light which strikes the surface depending
    //         on the angle of the surface to the light.
    float projection_factor(vec4 position, vec4 surface_normal) const {
        return glm::dot(this->normalised_dir, surface_normal);
    }

    // The shadow ray depends only on the direction of the light source, since
    // it has no position.
    //
    // return: a shadow ray from the point and the light source. Or, returns
    //         nothing if the light does not cast shadows.
    optional<Ray> ray_from(vec4 point) const {
        return Ray(point, this->normalised_dir * this->shadow_ray_len, 0);
    }

    // return: a number of randomly selected shadow rays from an area around
    //         the light to the point. Or, returns nothing if the light does
    //         not cast shadows.
    vector<Ray> random_shadow_rays_from(vec4 point, int num) const {
        // The center of the sphere used to draw shadow rays to.
        vec3 center_3d = vec3(point - this->normalised_dir);

        vector<Ray> rays;
        for (int i=0; i<num; i++) {
            vec3 point_in_sphere_3d = random_in_sphere(center_3d, this->radius);
            vec4 point_in_sphere = project_to_4D(point_in_sphere_3d);

            vec4 dir = (point - point_in_sphere) * this->shadow_ray_len;
            rays.push_back(Ray(point, dir, 0));
        }

        return rays;
    }
};

#endif // DIRECTIONAL_LIGHT_H
