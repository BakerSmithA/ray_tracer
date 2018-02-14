#include <glm/glm.hpp>
#include <vector>
#include "../geometry/triangle.h"
#include "../geometry/sphere.h"
#include "../lights/point_light.h"
#include "../lights/ambient_light.h"
#include "../shaders/diffuse.h"
#include "../shaders/mirror.h"
#include "../shaders/mix.h"
#include "../shaders/refraction.h"
#include "../shaders/fresnel.h"
#include "../shaders/glass.h"

using std::vector;

#ifndef CORNEL_BOX_H
#define CORNEL_BOX_H

// return: all the triangles of the cornel box in the volume:
// 			-1 <= x <= +1
// 			-1 <= y <= +1
// 			-1 <= z <= +1
vector<Primitive*> cornel_box_triangles() {
	using glm::vec3;
	using glm::vec4;

	// Defines colors:
	const vec3 red = vec3(0.75f, 0.15f, 0.15f);
	const vec3 yellow = vec3(0.75f, 0.75f, 0.15f);
	const vec3 green = vec3(0.15f, 0.75f, 0.15f);
	const vec3 cyan = vec3(0.15f, 0.75f, 0.75f);
	const vec3 blue = vec3(0.15f, 0.15f, 0.75f);
	const vec3 purple = vec3(0.75f, 0.15f, 0.75f);
	const vec3 white = vec3(0.75f, 0.75f, 0.75f);

	float prop = 0.6;
	const Shader *red_diffuse = new Mix(new Mirror(), new Diffuse(red), prop); // new Refraction(0.66); //new Fresnel(new Refraction(0.66), new Mirror());
	const Shader *yellow_diffuse = new Mix(new Mirror(), new Diffuse(yellow), prop);
	const Shader *green_diffuse = new Mix(new Mirror(), new Diffuse(green), prop);
	const Shader *cyan_diffuse = new Mix(new Mirror(), new Diffuse(cyan), prop);
	const Shader *blue_diffuse = new Mix(new Mirror(), new Diffuse(blue), prop);
	const Shader *purple_diffuse = new Mix(new Mirror(), new Diffuse(purple), prop);
	const Shader *white_diffuse = new Mix(new Mirror(), new Diffuse(white), prop);

	// const Shader *red_diffuse = new Diffuse(red);
	// const Shader *yellow_diffuse = new Diffuse(yellow);
	// const Shader *green_diffuse = new Diffuse(green);
	// const Shader *cyan_diffuse = new Diffuse(cyan);
	// const Shader *blue_diffuse = new Diffuse(blue);
	// const Shader *purple_diffuse = new Diffuse(purple);
	// const Shader *white_diffuse = new Diffuse(white);

	vector<Primitive*> triangles;
	triangles.clear();
	triangles.reserve(5*2*3);

	// ---------------------------------------------------------------------------
	// Room

	float L = 555;			// Length of Cornell Box side.

	vec4 A(L,0,0,1);
	vec4 B(0,0,0,1);
	vec4 C(L,0,L,1);
	vec4 D(0,0,L,1);

	vec4 E(L,L,0,1);
	vec4 F(0,L,0,1);
	vec4 G(L,L,L,1);
	vec4 H(0,L,L,1);

	// Floor:
	triangles.push_back(new Triangle(C, B, A, green_diffuse));
	triangles.push_back(new Triangle(C, D, B, green_diffuse));

	// Left wall
	triangles.push_back(new Triangle(A, E, C, purple_diffuse));
	triangles.push_back(new Triangle(C, E, G, purple_diffuse));

	// Right wall
	triangles.push_back(new Triangle(F, B, D, yellow_diffuse));
	triangles.push_back(new Triangle(H, F, D, yellow_diffuse));

	// Ceiling
	triangles.push_back(new Triangle(E, F, G, cyan_diffuse));
	triangles.push_back(new Triangle(F, H, G, cyan_diffuse));

	// Back wall
	triangles.push_back(new Triangle(G, D, C, white_diffuse));
	triangles.push_back(new Triangle(G, H, D, white_diffuse));

	// ---------------------------------------------------------------------------
	// Short block

	A = vec4(290,0,114,1);
	B = vec4(130,0, 65,1);
	C = vec4(240,0,272,1);
	D = vec4(82,0,225,1);

	E = vec4(290,165,114,1);
	F = vec4(130,165, 65,1);
	G = vec4(240,165,272,1);
	H = vec4(82,165,225,1);

	// Front
	triangles.push_back(new Triangle(E,B,A,red_diffuse));
	triangles.push_back(new Triangle(E,F,B,red_diffuse));

	// Front
	triangles.push_back(new Triangle(F,D,B,red_diffuse));
	triangles.push_back(new Triangle(F,H,D,red_diffuse));

	// BACK
	triangles.push_back(new Triangle(H,C,D,red_diffuse));
	triangles.push_back(new Triangle(H,G,C,red_diffuse));

	// LEFT
	triangles.push_back(new Triangle(G,E,C,red_diffuse));
	triangles.push_back(new Triangle(E,A,C,red_diffuse));

	// TOP
	triangles.push_back(new Triangle(G,F,E,red_diffuse));
	triangles.push_back(new Triangle(G,H,F,red_diffuse));

	// ---------------------------------------------------------------------------
	// Tall block

	A = vec4(423,0,247,1);
	B = vec4(265,0,296,1);
	C = vec4(472,0,406,1);
	D = vec4(314,0,456,1);

	E = vec4(423,330,247,1);
	F = vec4(265,330,296,1);
	G = vec4(472,330,406,1);
	H = vec4(314,330,456,1);

	// Front
	triangles.push_back(new Triangle(E,B,A,blue_diffuse));
	triangles.push_back(new Triangle(E,F,B,blue_diffuse));

	// Front
	triangles.push_back(new Triangle(F,D,B,blue_diffuse));
	triangles.push_back(new Triangle(F,H,D,blue_diffuse));

	// BACK
	triangles.push_back(new Triangle(H,C,D,blue_diffuse));
	triangles.push_back(new Triangle(H,G,C,blue_diffuse));

	// LEFT
	triangles.push_back(new Triangle(G,E,C,blue_diffuse));
	triangles.push_back(new Triangle(E,A,C,blue_diffuse));

	// TOP
	triangles.push_back(new Triangle(G,F,E,blue_diffuse));
	triangles.push_back(new Triangle(G,H,F,blue_diffuse));

	// ----------------------------------------------
	// Scale to the volume [-1,1]^3
	vector<Primitive*> new_triangles;

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

		new_triangles.push_back(new Triangle(new_v0, new_v1, new_v2, triangle->shader));
	}

	return new_triangles;
}

// return: the lights in the cornel box.
vector<Light*> cornel_box_lights() {
	vector<Light*> lights;

	lights.push_back(new PointLight(vec3(18, 18, 18), vec4(0, -0.5, -0.7, 1.0)));
	//lights.push_back(new PointLight(vec3(18, 18, 18), vec4(0, 0.3, -0.7, 1.0)));
	lights.push_back(new AmbientLight(vec3(0.3, 0.2, 0.2)));

	return lights;
}

// return: a cornel box scene.
Scene cornel_box() {
	vector<Primitive*> triangles = cornel_box_triangles();
	triangles.push_back(new Sphere(vec4(-0, 0.3, -0.9, 1.0), 0.3, new Glass()));
	return Scene(triangles, cornel_box_lights());
}

#endif // CORNEL_BOX_H
