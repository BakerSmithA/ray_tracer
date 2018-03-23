#ifndef STAR_MODEL_H
#define STAR_MODEL_H

// return: a star model.
Object *star_model() {
    Shader *shader = Projection::spherical("../texture_files/sun_large.bmp");

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.3, shader);
	return new Object(1, primitives);
}

// return: a star model.
Object *star_atmosphere() {
	// The transparency of the smoke for the distance a ray travelled through.
	auto smoke_transparency = [=](float smoke_dist) {
		return 1 - clamp(pow(smoke_dist, 3) * 5, 0.0, 1.0);
	};

    Shader *shader = new Smoke(vec3(1, 0, 0), smoke_transparency);

	Primitive **primitives = new Primitive*[1];
	primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.39, shader);
	return new Object(1, primitives);
}

// return: all the objects in the cornel box.
const Object **star_objects() {
	const Object **objects = new const Object*[3];
	objects[0] = star_model();
	objects[1] = star_atmosphere();
	objects[2] = star_map();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> star_lights() {
	vector<Light*> lights;

	vec4 pos = vec4(0, -0.5, 0, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	PointLight *light = new PointLight(col, pos, radius);

	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene star_scene() {
	return Scene(3, star_objects(), star_lights());
}

#endif // STAR_MODEL_H
