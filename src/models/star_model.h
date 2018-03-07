#ifndef STAR_MODEL_H
#define STAR_MODEL_H

// return: a star model.
Object *star_model() {
	vector<Primitive*> primitives;

    Shader *shader = Texture::spherical("../textures/sun.bmp");
	primitives.push_back(new Sphere(vec4(0, 0, 0, 1.0), 0.3, shader));

	return new Object(primitives);
}

// return: a star model.
Object *star_atmosphere() {
	vector<Primitive*> primitives;

    Shader *shader = new Smoke(vec3(1, 0, 0));
	primitives.push_back(new Sphere(vec4(0, 0, 0, 1.0), 0.39, shader));

	return new Object(primitives);
}

// return: all the objects in the cornel box.
vector<Object*> star_objects() {
	vector<Object*> objects;
	objects.push_back(star_model());
	objects.push_back(star_atmosphere());
	return objects;
}

// return: the lights in the cornel box.
vector<PointLight*> star_lights() {
	vector<PointLight*> lights;

	vec4 pos = vec4(0, -0.5, 0, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	PointLight *light = new PointLight(col, pos, radius);

	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene star_scene() {
	return Scene(star_objects(), star_lights());
}

#endif // STAR_MODEL_H
