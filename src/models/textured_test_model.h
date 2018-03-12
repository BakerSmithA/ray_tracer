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
#include "../shaders/texture.h"

using std::vector;
using glm::vec3;
using glm::vec4;

#ifndef TEXTURED_TEST_MODEL_H
#define TEXTURED_TEST_MODEL_H

// return: the floor of the box.
Object *floor() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Texture::planar("../textures/metal.bmp", planar_y);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader));
	triangles.push_back(new Triangle(C, D, B, shader));

	return new Object(scaled_triangles(triangles));
}

// return: the left wall of the box.
Object *left_wall() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Texture::planar("../textures/bricks.bmp", planar_x);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(A, E, C, shader));
	triangles.push_back(new Triangle(C, E, G, shader));

	return new Object(scaled_triangles(triangles));
}

// return: the right wall of the box.
Object *right_wall() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Texture::planar("../textures/bricks.bmp", planar_x);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(F, B, D, shader));
	triangles.push_back(new Triangle(H, F, D, shader));

	return new Object(scaled_triangles(triangles));
}

// return: the back wall of the box.
Object *back_wall() {
	const vec3 col = vec3(1, 1, 1);

	const Shader *diffuse_shader = new Diffuse(col);
	const Shader *tex_shader = Texture::planar("../textures/bricks.bmp", planar_z);
	const Shader *shader = Mix::multiply(diffuse_shader, tex_shader);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(G, D, C, shader));
	triangles.push_back(new Triangle(G, H, D, shader));

	return new Object(scaled_triangles(triangles));
}

// return: the ceiling of the cornel box.
Object *ceiling() {
	const vec3 col = vec3(0.75f, 0.75f, 0.75f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(E, F, G, shader));
	triangles.push_back(new Triangle(F, H, G, shader));

	return new Object(scaled_triangles(triangles));
}

// return: the short (red) block in the cornel box.
Object *short_block() {
	vec4 A = vec4(290,0,114,1);
	vec4 B = vec4(130,0, 65,1);
	vec4 C = vec4(240,0,272,1);
	vec4 D = vec4(82,0,225,1);

	vec4 E = vec4(290,165,114,1);
	vec4 F = vec4(130,165, 65,1);
	vec4 G = vec4(240,165,272,1);
	vec4 H = vec4(82,165,225,1);

	const vec3 col = vec3(0.75f, 0.15f, 0.15f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;

	// Front
	triangles.push_back(new Triangle(E,B,A,shader));
	triangles.push_back(new Triangle(E,F,B,shader));

	// Front
	triangles.push_back(new Triangle(F,D,B,shader));
	triangles.push_back(new Triangle(F,H,D,shader));

	// BACK
	triangles.push_back(new Triangle(H,C,D,shader));
	triangles.push_back(new Triangle(H,G,C,shader));

	// LEFT
	triangles.push_back(new Triangle(G,E,C,shader));
	triangles.push_back(new Triangle(E,A,C,shader));

	// TOP
	triangles.push_back(new Triangle(G,F,E,shader));
	triangles.push_back(new Triangle(G,H,F,shader));

	return new Object(scaled_triangles(triangles));
}

// return: the tall (blue) block in the cornel box.
Object *tall_block() {
	vec4 A = vec4(423,0,247,1);
	vec4 B = vec4(265,0,296,1);
	vec4 C = vec4(472,0,406,1);
	vec4 D = vec4(314,0,456,1);

	vec4 E = vec4(423,330,247,1);
	vec4 F = vec4(265,330,296,1);
	vec4 G = vec4(472,330,406,1);
	vec4 H = vec4(314,330,456,1);

	const vec3 col = vec3(0.15f, 0.15f, 0.75f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;

	// Front
	triangles.push_back(new Triangle(E,B,A,shader));
	triangles.push_back(new Triangle(E,F,B,shader));

	// Front
	triangles.push_back(new Triangle(F,D,B,shader));
	triangles.push_back(new Triangle(F,H,D,shader));

	// BACK
	triangles.push_back(new Triangle(H,C,D,shader));
	triangles.push_back(new Triangle(H,G,C,shader));

	// LEFT
	triangles.push_back(new Triangle(G,E,C,shader));
	triangles.push_back(new Triangle(E,A,C,shader));

	// TOP
	triangles.push_back(new Triangle(G,F,E,shader));
	triangles.push_back(new Triangle(G,H,F,shader));

	return new Object(scaled_triangles(triangles));
}

// return: a glass sphere.
Object *large_sphere() {
	vector<Primitive*> primitives;
	primitives.push_back(new Sphere(vec4(-0, 0.3, -0.9, 1.0), 0.3, new Glass()));

	return new Object(primitives);
}

Object *small_sphere() {
	vector<Primitive*> primitives;
	primitives.push_back(new Sphere(vec4(-0, -0.2, -0.8, 1.0), 0.2, new Glass()));

	return new Object(primitives);
}

// return: all the objects in the cornel box.
vector<Object*> objects() {
	vector<Object*> objects;

	objects.push_back(floor());
	objects.push_back(left_wall());
	objects.push_back(right_wall());
	objects.push_back(back_wall());
	objects.push_back(ceiling());
	// objects.push_back(short_block());
	// objects.push_back(tall_block());
	// objects.push_back(large_sphere());
	// objects.push_back(small_sphere());

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
	return Scene(objects(), lights());
}

#endif // TEXTURED_TEST_MODEL_H
