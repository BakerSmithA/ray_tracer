#pragma once

#include "../shaders/gravitational_lens.h"

namespace gravitational_lens {
    // return: a green sphere.
    Object *black_hole() {
    	Primitive **primitives = new Primitive*[1];

        float strength = 0.002f;
    	Shader *shader = new GravitationalLens(strength, 1.0f);
    	primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), 0.5f, shader);

    	return new Object(1, primitives);
    }

    // return: the back wall of the box.
    Object *back_wall() {
    	const Shader *shader = Projection::planar("../texture_files/metal.bmp", planar_z);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(G, D, C, shader));
    	triangles.push_back(new Triangle(G, H, D, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: all the objects in the scene.
    const Object **objects() {
    	const Object **objects = new const Object*[1];
    	objects[0] = black_hole();
        objects[1] = back_wall();
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
    	return Scene(2, objects(), lights());
    }
}
