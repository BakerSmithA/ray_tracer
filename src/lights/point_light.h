#include <math.h>
#include "../geometry/random.h"
#include "../geometry/projection.h"
#include "specular_light.h"

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

// Models a point light which radiates light outwards.
class PointLight: public SpecularLight {
public:
    const vec4 pos;
    // Used to determine how blurry shadows from the light should be.
    // A larger radius will produce blurrier shadows.
    const float radius;

    PointLight(vec3 color, vec4 pos, float radius):
        SpecularLight(color), pos(pos), radius(radius) {
    }

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of light at the position in the scene.
    vec3 intensity(vec4 point, vec4 surface_normal) const {
        // The distance from the light source to the intersection, i.e. the radius of the sphere.
        vec4 shadow_ray = this->pos - point;
        // The intensity of light is inversely proportional to the distance squared.
        float dist = length(shadow_ray);
        float surface_area_at_radius_r = 4 * M_PI * dist * dist;
        vec3 intensity = this->color / surface_area_at_radius_r;

        return intensity;
    }

    // return: a shadow ray from the point and the light source. This is
    //         only used if the light casts shadows.
    Ray ray_from(vec4 point) const {
        // The ray can only be used to check obstructions between the point and
        // light. Therefore it cannot bounce.
        return Ray(point, this->pos - point, 0);
    }

    // return: the given number of rays to points within the radius of the light.
    vector<Ray> random_shadow_rays_from(vec4 point, int num) const {
        // The ray can only be used to check obstructions between the point and
        // light. Therefore it cannot bounce.
        vec3 center_3d = vec3(this->pos);

        vector<Ray> rays;
        for (int i=0; i<num; i++) {
            vec3 point_in_sphere_3d = random_in_sphere(center_3d, this->radius);
            vec4 point_in_sphere = project_to_4D(point_in_sphere_3d);

            rays.push_back(Ray(point, point_in_sphere - point, 0));
        }

        return rays;
    }
};

#endif // POINT_LIGHT_H
