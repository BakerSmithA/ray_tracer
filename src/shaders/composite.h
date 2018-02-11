#include <glm/glm.hpp>
#include "shader.h"

using glm::mix;

#ifndef COMPOSITE_SHADER_H
#define COMPOSITE_SHADER_H

// Mixes two shaders together with given ratio.
class Composite: public Shader {
public:
    vector<Shader*> shaders;

    Composite(vector<Shader*> shaders): shaders(shaders) {}

    // return: the color of the intersected surface, as illuminated by a specific light.
    vec3 color(vec4 position, const Triangle &tri, const Ray &incoming, const Scene &scene, Light &light) const {        
        vec3 intensity(0,0,0);
        for(Shader* shader : shaders){
            intensity += shader->color(position, tri, incoming, scene, light);
        }
        return intensity;
    }
};

#endif // COMPOSITE_SHADER_H
