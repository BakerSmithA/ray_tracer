#ifndef SATURN_MODEL_H
#define SATURN_MODEL_H

// return: a star model.
Object *saturn_model() {
	vector<Primitive*> primitives;

    // The transparency of the smoke for the distance a ray travelled through.
    auto atmosphere_transparency = [=](float smoke_dist) {
        // The maximum distance a ray can travel through the atmosphere before
        // not emerging the other side.
        const float max_dist = 0.3f;

        if (smoke_dist <= max_dist) {
            return 1 - (smoke_dist / max_dist);
        }
        return 0.0f;
    };

    Shader *atmosphere = new Smoke(vec3(1, 1, 1), atmosphere_transparency);
    Shader *texture = Texture::spherical("../textures/jupiter.bmp");
    Shader *lighting = new Diffuse(vec3(1, 1, 1));

    Shader *combine = Mix::multiply(atmosphere, texture);
    Shader *shader = Mix::multiply(combine, lighting);

	primitives.push_back(new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.3, shader));

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
vector<PointLight*> saturn_lights() {
	vector<PointLight*> lights;

	vec4 pos = vec4(-1.0, -0.5, -0.7, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.1;

	PointLight *light = new PointLight(col, pos, radius);

	lights.push_back(light);

	return lights;
}

// return: a scene containing a star.
Scene saturn_scene() {
	return Scene(saturn_objects(), saturn_lights());
}

#endif // SATURN_MODEL_H
