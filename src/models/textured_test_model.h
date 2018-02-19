#include <glm/glm.hpp>
#include <vector>
#include "../geometry/triangle.h"
#include "../geometry/sphere.h"
#include "../geometry/object.h"
#include "../lights/point_light.h"
#include "../shaders/diffuse.h"
#include "../shaders/blinn_specular.h"
#include "../shaders/phong_specular.h"
#include "../shaders/mirror.h"
#include "../shaders/mix.h"
#include "../shaders/refraction.h"
#include "../shaders/fresnel.h"
#include "../shaders/glass.h"
#include "../shaders/flat_color.h"
#include "../shaders/smoke.h"

using std::vector;
using glm::vec3;
using glm::vec4;

#ifndef TEXTURED_TEST_MODEL_H
#define TEXTURED_TEST_MODEL_H

// return: the floor of the box.
Object *floor() {
	const vec3 col = vec3(0.75f, 0.75f, 0.75f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader, 0));
	triangles.push_back(new Triangle(C, D, B, shader, 0));

	return new Object(scaled_triangles(triangles));
}

// return: the left wall of the box.
Object *left_wall() {
	const vec3 col = vec3(0.75f, 0.15f, 0.75f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(A, E, C, shader, 0));
	triangles.push_back(new Triangle(C, E, G, shader, 0));

	return new Object(scaled_triangles(triangles));
}

// return: the right wall of the box.
Object *right_wall() {
	const vec3 col = vec3(0.75f, 0.75f, 0.15f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(F, B, D, shader, 0));
	triangles.push_back(new Triangle(H, F, D, shader, 0));

	return new Object(scaled_triangles(triangles));
}

// return: the back wall of the box.
Object *back_wall() {
	const vec3 col = vec3(0.75f, 0.75f, 0.75f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(G, D, C, shader, 0));
	triangles.push_back(new Triangle(G, H, D, shader, 0));

	return new Object(scaled_triangles(triangles));
}

// return: the ceiling of the cornel box.
Object *ceiling() {
	const vec3 col = vec3(0.15f, 0.75f, 0.75f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(E, F, G, shader, 0));
	triangles.push_back(new Triangle(F, H, G, shader, 0));

	return new Object(scaled_triangles(triangles));
}

// return: all the objects in the cornel box.
vector<Object*> objects() {
	vector<Object*> objects;

	objects.push_back(floor());
	objects.push_back(left_wall());
	objects.push_back(right_wall());
	objects.push_back(back_wall());
	objects.push_back(ceiling());

	return objects;
}

// return: the lights in the cornel box.
vector<PointLight*> lights() {
	vector<PointLight*> lights;

	lights.push_back(new PointLight(vec3(18, 18, 18), vec4(0, -0.5, -0.7, 1.0)));

	return lights;
}

// return: the cornel box scene.
Scene textured_test_scene() {
	return Scene(objects(), lights());
}

#endif // TEXTURED_TEST_MODEL_H
