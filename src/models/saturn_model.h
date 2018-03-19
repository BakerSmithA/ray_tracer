#include "../geometry/primitives/disc.h"

#ifndef SATURN_MODEL_H
#define SATURN_MODEL_H

// return: a star model.
Object *saturn_model() {
	const float sphere_radius = 0.3f;
	const float sphere_diameter = 2 * sphere_radius;

    // The transparency of the smoke for the distance a ray travelled through.
    auto atmosphere_transparency = [=](float smoke_dist) {
		// The larger the number, the blurrier the edges of the planet will be.
		// float edge_blur = 0.6f;
		float edge_blur = 0.1f;
		return 1 - (pow(smoke_dist, edge_blur) / pow(sphere_diameter, edge_blur));
    };

    Shader *atmosphere = new Smoke(vec3(1, 1, 1), atmosphere_transparency);
    Shader *texture = Texture::spherical("../textures/saturn.bmp");
    Shader *lighting = new Diffuse(vec3(1, 1, 1));

    Shader *combine = Mix::multiply(atmosphere, texture);
    Shader *shader = Mix::multiply(combine, lighting);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), sphere_radius, shader);
	return new Object(1, primitives);
}

Object *saturn_rings() {
	float inner_r = 0.47f;
	float outer_r = 0.64f;
	vec4 normal_dir = vec4(0.0f, 1.0f, 0.2f, 1.0f);
	vec4 center = vec4(0.1, 0, -0.4, 1.0);

	Shader *shader = Texture::planar("../textures/saturn_rings.bmp", planar_y);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Disc(inner_r, outer_r, normal_dir, center, shader);
	return new Object(1, primitives);
}

// return: a spherical star map.
Object *star_map() {
	Shader *shader = Texture::spherical("../textures/star_sphere_map.bmp");

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), 2.3, shader);
	return new Object(1, primitives);
}

// return: all the objects in the cornel box.
const Object **saturn_objects() {
	const Object **objects = new const Object*[3];
	objects[0] = saturn_model();
	objects[1] = saturn_rings();
	objects[2] = star_map();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> saturn_lights() {
	vector<Light*> lights;

	vec3 col = vec3(6, 6, 6);
	vec4 dir = vec4(-0.5, -0.1f, -0.1f, 1.0f);
	DirectionalLight *light = new DirectionalLight(col, dir, 1.0f, 0.03f);
	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene saturn_scene() {
	return Scene(3, saturn_objects(), saturn_lights());
}

#endif // SATURN_MODEL_H
