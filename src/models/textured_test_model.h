#include <glm/glm.hpp>
#include <vector>
#include "../geometry/primitives/triangle.h"
#include "../geometry/primitives/sphere.h"
#include "../geometry/object.h"
#include "../lights/point_light.h"
#include "../shaders/diffuse.h"
#include "../shaders/blinn.h"
#include "../shaders/mirror.h"
#include "../shaders/mix.h"
#include "../shaders/refraction.h"
#include "../shaders/fresnel.h"
#include "../shaders/glass.h"
#include "../shaders/flat_color.h"
#include "../shaders/smoke.h"
#include "../shaders/projection.h"

using std::vector;
using glm::vec3;
using glm::vec4;

#ifndef TEXTURED_TEST_MODEL_H
#define TEXTURED_TEST_MODEL_H

// return: the floor of the box.
Object *floor() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Projection::planar("../texture_files/metal.bmp", planar_y);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader));
	triangles.push_back(new Triangle(C, D, B, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the left wall of the box.
Object *left_wall() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Projection::planar("../texture_files/bricks.bmp", planar_x);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(A, E, C, shader));
	triangles.push_back(new Triangle(C, E, G, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the right wall of the box.
Object *right_wall() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Projection::planar("../texture_files/bricks.bmp", planar_x);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(F, B, D, shader));
	triangles.push_back(new Triangle(H, F, D, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the back wall of the box.
Object *back_wall() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Projection::planar("../texture_files/bricks.bmp", planar_z);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(G, D, C, shader));
	triangles.push_back(new Triangle(G, H, D, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the ceiling of the cornel box.
Object *ceiling() {
	const vec3 col = vec3(0.75f, 0.75f, 0.75f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(E, F, G, shader));
	triangles.push_back(new Triangle(F, H, G, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: all the objects in the cornel box.
const Object **objects(int *num_objects) {
	*num_objects = 5;
	const Object **objects = new const Object*[5];

	objects[0] = floor();
	objects[1] = left_wall();
	objects[2] = right_wall();
	objects[3] = back_wall();
	objects[4] = ceiling();

	return objects;
}

// return: the lights in the cornel box.
vector<Light*> lights() {
	vector<Light*> lights;

	vec4 pos = vec4(0, -0.5, -0.7, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	PointLight *light = new PointLight(col, pos, radius);

	lights.push_back(light);

	return lights;
}

// return: the cornel box scene.
Scene textured_test_scene() {
	int num_objects;
	return Scene(num_objects, objects(&num_objects), lights());
}

#endif // TEXTURED_TEST_MODEL_H
