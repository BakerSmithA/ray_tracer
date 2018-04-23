#pragma once

#include "../shaders/gravitational_lens.h"

namespace gravitational_lens {
    // return: a green sphere.
    Object *black_hole() {
    	Primitive **primitives = new Primitive*[1];

    	Shader *shader = new GravitationalLens();
    	primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), 1.0, shader);

    	return new Object(1, primitives);
    }

    // return: all the objects in the scene.
    const Object **objects() {
    	const Object **objects = new const Object*[1];
    	objects[0] = black_hole();
    	return objects;
    }

    // return: the lights in the sphere scene.
    vector<Light*> lights() {
    	vector<Light*> lights;

    	AmbientLight *ambient = new AmbientLight(vec3(1.0, 1.0, 1.0));
    	lights.push_back(ambient);

    	return lights;
    }

    // return: a scene containing a sphere.
    Scene scene() {
    	return Scene(1, objects(), lights());
    }
}
