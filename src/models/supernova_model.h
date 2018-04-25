#pragma once

#include "../textures/ramp.h"

namespace supernova_model {
    Object *cloud() {
        Shader *shader = new Diffuse(vec3(1,0,0));

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.6, shader);
        return new Object(1, primitives);
    }

    const Object **objects() {
        const Object **objects = new const Object*[1];
        objects[0] = cloud();
        return objects;
    }

    // return: the lights in the cornel box.
    vector<Light*> lights() {
    	vector<Light*> lights;

    	vec4 light1_pos = vec4(1.25, -2.0, 1.0, 1.0);
    	vec3 light1_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 35.0f;
    	PointLight *light1 = new PointLight(light1_col, light1_pos, 0.05f, 0.25f);

    	lights.push_back(light1);
    	return lights;
    }

    Scene scene() {
    	return Scene(1, objects(), lights());
    }
}
