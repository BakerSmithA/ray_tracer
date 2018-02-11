#include <glm/glm.hpp>
#include <vector>
#include "../geometry/triangle.h"
#include "../lights/phong_point_light.h"
#include "../lights/blinn_point_light.h"
#include "../lights/diffuse_point_light.h"
#include "../lights/ambient_light.h"
#include "../shaders/diffuse.h"
#include "../shaders/composite.h"
#include "../shaders/specular.h"
#include "../shaders/ambient.h"
#include "../shaders/mirror.h"
#include "../shaders/mix.h"

using std::vector;

#ifndef CORNEL_BOX_H
#define CORNEL_BOX_H

// return: all the triangles of the cornel box in the volume:
// 			-1 <= x <= +1
// 			-1 <= y <= +1
// 			-1 <= z <= +1
vector<Triangle> cornel_box_triangles() {
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

	// const Shader *red_diffuse = new Composite({ new Ambient(red), new Diffuse(red) });
	// const Shader *yellow_diffuse = new Composite({ new Ambient(yellow), new Diffuse(yellow) });
	// const Shader *green_diffuse = new Composite({ new Ambient(green), new Diffuse(green) });
	// const Shader *cyan_diffuse = new Composite({ new Ambient(cyan), new Diffuse(cyan) });
	// const Shader *blue_diffuse = new Composite({ new Ambient(blue), new Diffuse(blue) });
	// const Shader *purple_diffuse = new Composite({ new Ambient(purple), new Diffuse(purple) });
	// const Shader *white_diffuse = new Composite({ new Ambient(white), new Diffuse(white) });

	const Shader *red_diffuse_specular = new Composite({ new Ambient(red), new Diffuse(red), new Specular(red) });
	const Shader *yellow_diffuse_specular = new Composite({ new Ambient(yellow), new Diffuse(yellow), new Specular(yellow) });
	const Shader *green_diffuse_specular = new Composite({ new Ambient(green), new Diffuse(green), new Specular(green) });
	const Shader *cyan_diffuse_specular = new Composite({ new Ambient(cyan), new Diffuse(cyan), new Specular(cyan) });
	const Shader *blue_diffuse_specular = new Composite({ new Ambient(blue), new Diffuse(blue), new Specular(blue) });
	const Shader *purple_diffuse_specular = new Composite({ new Ambient(purple), new Diffuse(purple), new Specular(purple) });
	const Shader *white_diffuse_specular = new Composite({ new Ambient(white), new Diffuse(white), new Specular(white) });

	// float prop = 0.5;
	// const Shader *red_diffuse_mirrior = new Mix(new Composite({ new Ambient(red), new Diffuse(red) }), new Mirror(), prop);
	// const Shader *yellow_diffuse_mirrior = new Mix(new Composite({ new Ambient(yellow), new Diffuse(yellow) }), new Mirror(), prop);
	// const Shader *green_diffuse_mirrior = new Mix(new Composite({ new Ambient(green), new Diffuse(green) }), new Mirror(), prop);
	// const Shader *cyan_diffuse_mirrior = new Mix(new Composite({ new Ambient(cyan), new Diffuse(cyan) }), new Mirror(), prop);
	// const Shader *blue_diffuse_mirrior = new Mix(new Composite({ new Ambient(blue), new Diffuse(blue) }), new Mirror(), prop);
	// const Shader *purple_diffuse_mirrior = new Mix(new Composite({ new Ambient(purple), new Diffuse(purple) }), new Mirror(), prop);
	// const Shader *white_diffuse_mirrior = new Mix(new Composite({ new Ambient(white), new Diffuse(white) }), new Mirror(), prop);


	vector<Triangle> triangles;
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
	triangles.push_back(Triangle(C, B, A, green_diffuse_specular));
	triangles.push_back(Triangle(C, D, B, green_diffuse_specular));

	// Left wall
	triangles.push_back(Triangle(A, E, C, purple_diffuse_specular));
	triangles.push_back(Triangle(C, E, G, purple_diffuse_specular));

	// Right wall
	triangles.push_back(Triangle(F, B, D, yellow_diffuse_specular));
	triangles.push_back(Triangle(H, F, D, yellow_diffuse_specular));

	// Ceiling
	triangles.push_back(Triangle(E, F, G, cyan_diffuse_specular));
	triangles.push_back(Triangle(F, H, G, cyan_diffuse_specular));

	// Back wall
	triangles.push_back(Triangle(G, D, C, white_diffuse_specular));
	triangles.push_back(Triangle(G, H, D, white_diffuse_specular));

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
	triangles.push_back(Triangle(E,B,A,red_diffuse_specular));
	triangles.push_back(Triangle(E,F,B,red_diffuse_specular));

	// Front
	triangles.push_back(Triangle(F,D,B,red_diffuse_specular));
	triangles.push_back(Triangle(F,H,D,red_diffuse_specular));

	// BACK
	triangles.push_back(Triangle(H,C,D,red_diffuse_specular));
	triangles.push_back(Triangle(H,G,C,red_diffuse_specular));

	// LEFT
	triangles.push_back(Triangle(G,E,C,red_diffuse_specular));
	triangles.push_back(Triangle(E,A,C,red_diffuse_specular));

	// TOP
	triangles.push_back(Triangle(G,F,E,red_diffuse_specular));
	triangles.push_back(Triangle(G,H,F,red_diffuse_specular));

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
	triangles.push_back(Triangle(E,B,A,blue_diffuse_specular));
	triangles.push_back(Triangle(E,F,B,blue_diffuse_specular));

	// Front
	triangles.push_back(Triangle(F,D,B,blue_diffuse_specular));
	triangles.push_back(Triangle(F,H,D,blue_diffuse_specular));

	// BACK
	triangles.push_back(Triangle(H,C,D,blue_diffuse_specular));
	triangles.push_back(Triangle(H,G,C,blue_diffuse_specular));

	// LEFT
	triangles.push_back(Triangle(G,E,C,blue_diffuse_specular));
	triangles.push_back(Triangle(E,A,C,blue_diffuse_specular));

	// TOP
	triangles.push_back(Triangle(G,F,E,blue_diffuse_specular));
	triangles.push_back(Triangle(G,H,F,blue_diffuse_specular));

	// ----------------------------------------------
	// Scale to the volume [-1,1]^3
	vector<Triangle> new_triangles;

	for(int i=0; i<triangles.size(); i++) {
		vec4 new_v0 = triangles[i].v0;
		vec4 new_v1 = triangles[i].v1;
		vec4 new_v2 = triangles[i].v2;

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

		new_triangles.push_back(Triangle(new_v0, new_v1, new_v2, triangles[i].shader));
	}

	return new_triangles;
}

// return: the lights in the cornel box.
vector<Light*> cornel_box_lights() {
	vector<Light*> lights;

	//lights.push_back(new PhongPointLight(vec3(50, 50, 50), vec4(0, -0.5, -0.5, 1.0), 1));
	lights.push_back(new BlinnPointLight(vec3(18, 18, 18), vec4(0, -0.5, -0.5, 1.0), 1));
	// lights.push_back(new DiffusePointLight(vec3(50, 50, 50), vec4(0, -0.5, -0.5, 1.0)));
	// lights.push_back(new AmbientLight(vec3(0.7, 0.7, 0.7)));

	return lights;
}

// return: a cornel box scene.
Scene cornel_box() {
	return Scene(cornel_box_triangles(), cornel_box_lights());
}

#endif // CORNEL_BOX_H
