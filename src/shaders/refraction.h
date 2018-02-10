#include <glm/glm.hpp>
#include "shader.hpp"

#ifndef REFRACTION_H
#define REFRACTION_H

// Models an ideal refraction surface, i.e. 100% of the light entering is
// refracted.
class Refraction: public Shader {
public:
    // The ratio n1/n2, assuming n1 is a vacuum. This indicates how much light
    // slows down when entering the medium.
    // float ray_velocity_ratio;
    //
    // Refraction(float ray_velocity_ratio): ray_velocity_ratio()

    // return: the color of the intersected surface, as illuminated by a
    //         specific light. Becomes the color of the point that the
    //         refracted ray hits.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, Light &light) const {
    }
};

#endif // REFRACTION_H
