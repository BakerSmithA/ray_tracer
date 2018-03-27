#include "../textures/uniform_3d.h"
#include "../textures/stack_3d.h"
#include "../shaders/volumetric.h"

#ifndef VOLUME_MODEL_H
#define VOLUME_MODEL_H

Object *volume_sphere_model() {
	Texture3d *texture = new Stack3d("../texture_files/cloud_frames.bmp", 12);

	const vec3 extinction_color = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 0.3f;
    const float primary_step_size = 0.015f;
	const float shadow_step_size = 0.02f;
    const float extinction_coefficient = 5.5f;
	const float scattering_coefficient = 0.135f;

    Shader *shader = new Volumetric(texture, extinction_color, primary_step_size, shadow_step_size, extinction_coefficient, scattering_coefficient);

    Primitive **primitives = new Primitive*[1];
    primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.6, shader);
    return new Object(1, primitives);
}

Object *sphere_inside_volume_model() {
	Shader *shader = new Diffuse(vec3(1,0,0));

	Primitive **primitives = new Primitive*[1];
    primitives[0] = new Sphere(vec4(0.1, 0, -0.1, 1.0), 0.2, shader);
    return new Object(1, primitives);
}

// return: all the objects in the cornel box.
const Object **volume_objects() {
	const Object **objects = new const Object*[2];
    objects[0] = volume_sphere_model();
	objects[1] = sphere_inside_volume_model();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> volume_lights() {
	vector<Light*> lights;

	float radius = 0.01;

	vec4 light1_pos = vec4(-2.0, -2.0, 1.0, 1.0);
	//vec3 light1_col = vec3(1.0f) * 25.0f;
	vec3 light1_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 35.0f;

	vec4 light2_pos = vec4(2.0, 2.0, 0.0, 1.0);
	vec3 light2_col = vec3(142, 135, 167) * vec3(1.0f/255.0f) * 7.0f;

	vec3 ambient_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 0.5f;

	PointLight *light1 = new PointLight(light1_col, light1_pos, radius, 0.25f);
	PointLight *light2 = new PointLight(light2_col, light2_pos, radius, 0.25f);
    AmbientLight *ambient = new AmbientLight(ambient_col);

	lights.push_back(light1);
	lights.push_back(light2);
    //lights.push_back(ambient);

	return lights;
}

// return: a scene containing a star.
Scene volume_scene() {
	return Scene(1, volume_objects(), volume_lights());
}

#endif // VOLUME_MODEL_H
