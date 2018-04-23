#include <glm/glm.hpp>
#include <vector>
#include "../geometry/primitives/triangle.h"
#include "../geometry/primitives/sphere.h"
#include "../geometry/object.h"
#include "../lights/point_light.h"
#include "../lights/directional_light.h"
#include "../lights/ambient_light.h"
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
#include "../textures/perlin_2d.h"
#include "../shaders/mask.h"

using std::vector;
using glm::vec3;
using glm::vec4;

#ifndef CORNEL_BOX_H
#define CORNEL_BOX_H

const float L = 555.0f;

vec4 A(L,0,0,1);
vec4 B(0,0,0,1);
vec4 C(L,0,L,1);
vec4 D(0,0,L,1);

vec4 E(L,L,0,1);
vec4 F(0,L,0,1);
vec4 G(L,L,L,1);
vec4 H(0,L,L,1);

// return: the triangles scaled to fit in a -1 to 1 box.
Primitive **scaled_triangles(vector<Triangle*> triangles) {
	Primitive **new_triangles = new Primitive*[triangles.size()];

	for(size_t i=0; i<triangles.size(); i++) {
		Triangle* triangle = static_cast<Triangle*>(triangles[i]);
		vec4 new_v0 = triangle->v0;
		vec4 new_v1 = triangle->v1;
		vec4 new_v2 = triangle->v2;

		new_v0 *= 2/L;
		new_v1 *= 2/L;
		new_v2 *= 2/L;

		new_v0 -= vec4(1,1,1,1);
		new_v1 -= vec4(1,1,1,1);
		new_v2 -= vec4(1,1,1,1);

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

// return: the shader to be used for an object in the cornel box.
Shader *cornel_shader(vec3 col) {
	float mix_prop = 0.7f;
	Shader *material = Mix::ratio(new Mirror(), new Diffuse(col), mix_prop);
	return material;
}

// return: the floor of the cornel box.
Object *cornel_floor() {
	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader));
	triangles.push_back(new Triangle(C, D, B, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the left wall of the cornel box.
Object *cornel_left_wall() {
	const vec3 col = vec3(1.0f, 0.0f, 0.0f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(A, E, C, shader));
	triangles.push_back(new Triangle(C, E, G, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the right wall of the cornel box.
Object *cornel_right_wall() {
	const vec3 col = vec3(0.0f, 1.0f, 0.0f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(F, B, D, shader));
	triangles.push_back(new Triangle(H, F, D, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the back wall of the cornel box.
Object *cornel_back_wall() {
	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
	const Shader *diffuse_shader = cornel_shader(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(G, D, C, diffuse_shader));
	triangles.push_back(new Triangle(G, H, D, diffuse_shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the ceiling of the cornel box.
Object *cornel_ceiling() {
	const vec3 col = vec3(1.0f, 1.0f, 1.0f);//vec3(0.15f, 0.75f, 0.75f);
	const Shader *shader = cornel_shader(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(E, F, G, shader));
	triangles.push_back(new Triangle(F, H, G, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the short (red) block in the cornel box.
Object *cornel_short_block() {
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

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the tall (blue) block in the cornel box.
Object *cornel_tall_block() {
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

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: a glass sphere.
Object *cornel_large_sphere() {
	Shader *shader = new Glass();

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.0, 0.3, -0.9, 1.0), 0.3, shader);
	return new Object(1, primitives);
}

Object *cornel_small_sphere() {
	// Shader* shader = Mix::ratio(new Mirror(), new Diffuse(vec3(1.0f, 0.97f, 0.45f)), 0.1f);

	int octaves = 8;
	const Texture<vec2> *noise = new Perlin<vec2>(octaves);
	const Shader *mask = Projection::planar(noise, planar_z);

	const Shader *s1 = new Diffuse(vec3(0.2,0.2,0.2));
	const Shader *s2 = new Mirror();

	Shader *shader = new Mask(s1, s2, mask);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.3, -0.2, -0.1, 1.0), 0.2, shader);
	return new Object(1, primitives);
}

// return: all the objects in the cornel box.
const Object **cornel_objects(int *num_objects) {
	*num_objects = 9;
	const Object **objects = new const Object*[9];

	objects[0] = cornel_floor();
	objects[1] = cornel_left_wall();
	objects[2] = cornel_right_wall();
	objects[3] = cornel_back_wall();
	objects[4] = cornel_ceiling();
	objects[5] = cornel_short_block();
	objects[6] = cornel_tall_block();
	objects[7] = cornel_large_sphere();
	objects[8] = cornel_small_sphere();

	return objects;
}

// return: the lights in the cornel box.
vector<Light*> cornel_lights() {
	vector<Light*> lights;

	vec4 pos = vec4(0, -0.5, -0.7, 1.0);
	// vec3 col = vec3(18, 18, 18);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	PointLight *light = new PointLight(col, pos, radius);
	// AmbientLight *ambient = new AmbientLight(vec3(0.15f, 0.15f, 0.15f));
	AmbientLight *ambient = new AmbientLight(vec3(0.2f, 0.2f, 0.2f));

	lights.push_back(light);
	lights.push_back(ambient);

	return lights;
}

// return: the cornel box scene.
Scene cornel_box() {
	int num_objects;
	return Scene(num_objects, cornel_objects(&num_objects), cornel_lights());
}

#endif // CORNEL_BOX_H
