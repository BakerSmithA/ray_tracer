#include "../textures/texture.h"
#include "../textures/constant_texture.h"
#include "../shaders/volumetric.h"

#ifndef VOLUME_MODEL_H
#define VOLUME_MODEL_H

// return: a cube with a volumetric shader.
Object *cube_volume_model() {
    const float ray_step_size = 0.01f;
    Texture<vec4> *texture = new ConstantTexture<vec4>(vec3(0.5f, 1.0f, 0.5f), 1.0f);
    Shader *shader = new Volumetric(texture, ray_step_size);

    vec4 A = vec4(290,0,114,1);
	vec4 B = vec4(130,0, 65,1);
	vec4 C = vec4(240,0,272,1);
	vec4 D = vec4(82,0,225,1);

	vec4 E = vec4(290,165,114,1);
	vec4 F = vec4(130,165, 65,1);
	vec4 G = vec4(240,165,272,1);
	vec4 H = vec4(82,165,225,1);

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
    const float ray_step_size = 0.01f;
    Texture<vec4> *texture = new ConstantTexture<vec4>(vec3(0.5f, 1.0f, 0.5f), 1.0f);
    Shader *shader = new Volumetric(texture, ray_step_size);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.1f, 0.0f, -0.4f, 1.0f), 0.2f, shader);
	return new Object(1, primitives);
}

// return: the floor of the volumetric model box.
Object *volume_floor() {
	const Shader *shader = new Diffuse(vec3(1.0, 0.0, 0.0));

	vector<Triangle*> triangles;
	triangles.push_back(new Triangle(C, B, A, shader));
	triangles.push_back(new Triangle(C, D, B, shader));

	return new Object(triangles.size(), scaled_triangles(triangles));
}

// return: all the objects in the cornel box.
const Object **volume_objects() {
	const Object **objects = new const Object*[3];
	objects[0] = cube_volume_model();
    objects[1] = sphere_volume_model();
    objects[2] = volume_floor();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> volume_lights() {
	vector<Light*> lights;

	// vec4 pos = vec4(0, -0.5, 0, 1.0);
	// vec3 col = vec3(18, 18, 18);
	// float radius = 0.1;

	//PointLight *light = new PointLight(col, pos, radius);
    AmbientLight *light = new AmbientLight(vec3(1.0, 1.0, 1.0));

	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene volume_scene() {
	return Scene(3, volume_objects(), volume_lights());
}

#endif // VOLUME_MODEL_H
