#include "../textures/uniform_3d.h"
#include "../textures/stacked_3d.h"
#include "../shaders/volumetric.h"

#ifndef VOLUME_MODEL_H
#define VOLUME_MODEL_H

// return: a cube with a volumetric shader.
Object *cube_volume_model() {
    const float extinction_coeff = 2.0f;
    const float ray_step_size = 0.01f;
    Texture3d *texture = new Stacked3d("../texture_files/cloud_frames.bmp", 12);
    Shader *shader = new Volumetric(texture, ray_step_size, extinction_coeff);

    float y = 0.0f;

    vec4 A = vec4(290,y,114,1);
	vec4 B = vec4(130,y, 65,1);
	vec4 C = vec4(240,y,272,1);
	vec4 D = vec4(82,y,225,1);

	vec4 E = vec4(290,165+y,114,1);
	vec4 F = vec4(130,165+y, 65,1);
	vec4 G = vec4(240,165+y,272,1);
	vec4 H = vec4(82,165+y,225,1);

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

Object *sphere_volume_model() {
    const float extinction_coeff = 1.5f;
    const float ray_step_size = 0.01f;
    Texture3d *texture = new Uniform3d(1.0f);
    Shader *shader = new Volumetric(texture, ray_step_size, extinction_coeff);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(-0.3f, 0.0f, 0.0f, 1.0f), 0.6f, shader);
	return new Object(1, primitives);
}

// return: the floor of the volumetric model box.
Object *volume_floor() {
    vec3 col = vec3(1.0f, 1.0f, 1.0f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader));
	triangles.push_back(new Triangle(C, D, B, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the left wall of the cornel box.
Object *volume_left_wall() {
	const vec3 col = vec3(1.0f, 0.0f, 0.0f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(A, E, C, shader));
	triangles.push_back(new Triangle(C, E, G, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the right wall of the cornel box.
Object *volume_right_wall() {
	const vec3 col = vec3(0.0f, 1.0f, 0.0f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(F, B, D, shader));
	triangles.push_back(new Triangle(H, F, D, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the back wall of the cornel box.
Object *volume_back_wall() {
	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(G, D, C, shader));
	triangles.push_back(new Triangle(G, H, D, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: the ceiling of the cornel box.
Object *volume_ceiling() {
	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
	const Shader *shader = new Diffuse(col);

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(E, F, G, shader));
	triangles.push_back(new Triangle(F, H, G, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: all the objects in the cornel box.
const Object **volume_objects() {
	const Object **objects = new const Object*[7];
    objects[0] = volume_floor();
    objects[1] = volume_left_wall();
    objects[2] = volume_right_wall();
    objects[3] = volume_back_wall();
    objects[4] = volume_ceiling();
    objects[5] = cube_volume_model();
    objects[6] = sphere_volume_model();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> volume_lights() {
	vector<Light*> lights;

	vec4 pos = vec4(0, -0.5, 0, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	PointLight *light = new PointLight(col, pos, radius);
    // AmbientLight *ambient = new AmbientLight(vec3(0.1, 0.1, 0.1));
    AmbientLight *ambient = new AmbientLight(vec3(1,1,1));

	//lights.push_back(light);
    lights.push_back(ambient);

	return lights;
}

// return: a scene containing a star.
Scene volume_scene() {
	return Scene(6, volume_objects(), volume_lights());
}

#endif // VOLUME_MODEL_H
