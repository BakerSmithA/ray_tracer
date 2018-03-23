#include "../textures/texture.h"
#include "../textures/constant_texture.h"
#include "../shaders/volumetric.h"

#ifndef VOLUME_MODEL_H
#define VOLUME_MODEL_H

// return: a star model.
Object *cube_volume_model() {
    const float ray_step_size = 0.01f;
    Texture<vec4> *texture = new ConstantTexture<vec4>(vec3(0.5f, 1.0f, 0.5f), 1.0f);
    Shader *shader = new Volumetric(texture, ray_step_size);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.1f, 0.0f, -0.4f, 1.0f), 0.5f, shader);
	return new Object(1, primitives);
}

// return: all the objects in the cornel box.
const Object **volume_objects() {
	const Object **objects = new const Object*[1];
	objects[0] = cube_volume_model();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> volume_lights() {
	vector<Light*> lights;

	vec4 pos = vec4(0, -0.5, 0, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	//PointLight *light = new PointLight(col, pos, radius);
    AmbientLight *light = new AmbientLight(vec3(1.0, 1.0, 1.0));

	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene volume_scene() {
	return Scene(1, volume_objects(), volume_lights());
}

#endif // VOLUME_MODEL_H
