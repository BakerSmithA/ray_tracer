#include "../geometry/primitives/disc.h"
#include "../textures/uniform_3d.h"
#include "../shaders/volumetric.h"

#ifndef SATURN_MODEL_H
#define SATURN_MODEL_H

// return: a star model.
Object *saturn_model() {
	Texture3d *volume_texture = new Uniform3d(1.0f);

	const vec3 extinction_color = vec3(0.0f);
    const float primary_step_size = 0.015f;
	const float shadow_step_size = 0.035f;
    const float extinction_coefficient = 20.0f;
	const float scattering_coefficient = 0.7f;

    Shader *volume = new Volumetric(volume_texture, extinction_color, primary_step_size, shadow_step_size, extinction_coefficient, scattering_coefficient);
	Shader *texture = Projection::spherical("../texture_files/jupiter.bmp");
	Shader *lighting = new Diffuse(vec3(1.0f));
	Shader *combine = Mix::multiply(volume, texture);
	Shader *shader = Mix::multiply(lighting, combine);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.3f, shader);
	return new Object(1, primitives);
}

Object *saturn_rings() {
	float inner_r = 0.47f;
	float outer_r = 0.64f;
	vec4 normal_dir = vec4(0.0f, 1.0f, 0.2f, 1.0f);
	vec4 center = vec4(0.1, 0, -0.4, 1.0);

	Shader *shader = Projection::planar("../texture_files/saturn_rings.bmp", planar_y);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Disc(inner_r, outer_r, normal_dir, center, shader);
	return new Object(1, primitives);
}

// return: a spherical star map.
Object *star_map() {
	Shader *shader = Projection::spherical("../texture_files/star_sphere_map.bmp");

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), 2.5, shader);
	return new Object(1, primitives);
}

// return: all the objects in the cornel box.
const Object **saturn_objects() {
	const Object **objects = new const Object*[3];
	objects[0] = saturn_model();
	objects[1] = star_map();
	objects[2] = saturn_rings();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> saturn_lights() {
	vector<Light*> lights;

	vec3 col = vec3(10, 10, 10);
	vec4 dir = vec4(-0.5, -0.1f, -0.1f, 1.0f);
	Light *light = new DirectionalLight(col, dir, 1.0f, 0.03f);
	Light *ambient = new AmbientLight(vec3(2.05f));

	lights.push_back(light);
	lights.push_back(ambient);

	return lights;
}

// return: a scene containing a star.
Scene saturn_scene() {
	return Scene(2, saturn_objects(), saturn_lights());
}

#endif // SATURN_MODEL_H
