#include "../textures/uniform_3d.h"
#include "../textures/stack_3d.h"
#include "../shaders/volumetric.h"

#ifndef VOLUME_MODEL_H
#define VOLUME_MODEL_H

// return: the triangles scaled to fit in a -1 to 1 box.
Primitive **converted_triangles(vector<Triangle*> triangles, float width) {
	Primitive **new_triangles = new Primitive*[triangles.size()];

	for(size_t i=0; i<triangles.size(); i++) {
		Triangle* triangle = triangles[i];

		vec4 new_v0 = triangle->v0;
		vec4 new_v1 = triangle->v1;
		vec4 new_v2 = triangle->v2;

		new_v0 *= 2;
		new_v1 *= 2;
		new_v2 *= 2;

		new_v0.x *= width;
		new_v1.x *= width;
		new_v2.x *= width;

		new_v0 -= vec4(1);
		new_v1 -= vec4(1);
		new_v2 -= vec4(1);

		new_v0.x -= width / 2;
		new_v1.x -= width / 2;
		new_v2.x -= width / 2;

		new_v0.x *= -1;
		new_v1.x *= -1;
		new_v2.x *= -1;

		new_v0.y *= -1;
		new_v1.y *= -1;
		new_v2.y *= -1;

		new_v0.w = 1.0;
		new_v1.w = 1.0;
		new_v2.w = 1.0;

		new_triangles[i] = new Triangle(new_v0, new_v1, new_v2, triangle->shader);
	}

	return new_triangles;
}

Object *volume_sphere_model() {
	Texture3d *texture = new Stack3d("../texture_files/cloud_frames.bmp", 12);

	const vec3 extinction_color = vec3(0.0f);
    const float primary_step_size = 0.015f;
	const float shadow_step_size = 0.02f;
    const float extinction_coefficient = 3.5f;
	const float scattering_coefficient = 2.0f;

    Shader *shader = new Volumetric(texture,
									extinction_color,
									primary_step_size,
									shadow_step_size,
									extinction_coefficient,
									scattering_coefficient);

    Primitive **primitives = new Primitive*[1];
    primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.6, shader);
    return new Object(1, primitives);
}

Object *sphere_inside_volume_model() {
	vec3 col = vec3(65, 87, 110) * vec3(1.0f/255.0f);
	Shader *shader = new Diffuse(col);

	Primitive **primitives = new Primitive*[1];
    primitives[0] = new Sphere(vec4(-0.4, 0.0, -0.2, 1.0), 0.5, shader);
    return new Object(1, primitives);
}

// return: the floor of the box.
Object *volume_floor() {
	vec4 A(1.0, 0.0, 0.0, 1.0);
	vec4 B(0.0, 0.0, 0.0, 1.0);
	vec4 C(1.0, 0.0, 0.7, 1.0);
	vec4 D(0.0, 0.0, 0.7, 1.0);

	const vec3 col = vec3(1, 1, 1);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader));
	triangles.push_back(new Triangle(C, D, B, shader));

	return new Object(triangles.size(), converted_triangles(triangles, 7.0f));
}

// return: all the objects in the cornel box.
const Object **volume_objects() {
	const Object **objects = new const Object*[3];
    objects[0] = volume_sphere_model();
	objects[1] = volume_floor();
	objects[2] = sphere_inside_volume_model();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> volume_lights() {
	vector<Light*> lights;

	vec4 light1_pos = vec4(1.25, -2.0, 1.0, 1.0);
	vec3 light1_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 35.0f;

	vec4 light2_pos = vec4(0.0, 1.0, 0.0, 1.0);
	vec3 light2_col = vec3(142, 135, 167) * vec3(1.0f/255.0f) * 1.15f;

	vec3 ambient_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 0.03f;

	PointLight *light1 = new PointLight(light1_col, light1_pos, 0.05f, 0.25f);
	PointLight *light2 = new PointLight(light2_col, light2_pos, 0.4f, 0.25f);
    AmbientLight *ambient = new AmbientLight(ambient_col);

	lights.push_back(light1);
	//lights.push_back(light2);
    //lights.push_back(ambient);

	return lights;
}

// return: a scene containing a star.
Scene volume_scene() {
	return Scene(2, volume_objects(), volume_lights());
}

#endif // VOLUME_MODEL_H
