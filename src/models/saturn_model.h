#ifndef SATURN_MODEL_H
#define SATURN_MODEL_H

// return: a star model.
Object *saturn_model() {
	vector<Primitive*> primitives;

	const float sphere_radius = 0.3f;
	const float sphere_diameter = 2 * sphere_radius;

    // The transparency of the smoke for the distance a ray travelled through.
    auto atmosphere_transparency = [=](float smoke_dist) {
		// The larger the number, the blurrier the edges of the planet will be.
		float edge_blur = 0.6f;
		return 1 - (pow(smoke_dist, edge_blur) / pow(sphere_diameter, edge_blur));
    };

    Shader *atmosphere = new Smoke(vec3(1, 1, 1), atmosphere_transparency);
    Shader *texture = Texture::spherical("../textures/jupiter.bmp");
    Shader *lighting = new Diffuse(vec3(1, 1, 1));

    Shader *combine = Mix::multiply(atmosphere, texture);
    Shader *shader = Mix::multiply(combine, lighting);

	primitives.push_back(new Sphere(vec4(0.1, 0, -0.4, 1.0), sphere_radius, shader));

	return new Object(primitives);
}

// return: all the objects in the cornel box.
vector<Object*> saturn_objects() {
	vector<Object*> objects;
	objects.push_back(saturn_model());
	objects.push_back(star_map());
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> saturn_lights() {
	vector<Light*> lights;
	//
	// vec4 pos = vec4(-1.0, -0.5, -0.7, 1.0);
	// vec3 col = vec3(20, 20, 20);
	// float radius = 0.1;
	//
	// PointLight *light = new PointLight(col, pos, radius);
	//
	// lights.push_back(light);
	//
	// return lights;

	vec3 col = vec3(2, 2, 2);
	vec4 dir = vec4(-0.5, -0.5, -0.8, 1);
	DirectionalLight *light = new DirectionalLight(col, dir, 1.0f, 0.1f);
	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene saturn_scene() {
	return Scene(saturn_objects(), saturn_lights());
}

#endif // SATURN_MODEL_H
