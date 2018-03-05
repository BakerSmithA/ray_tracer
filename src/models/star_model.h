#ifndef STAR_MODEL_H
#define STAR_MODEL_H

// return: a star model.
Object *star_model() {
	vector<Primitive*> primitives;

    Shader *shader = new Texture("../textures/bricks.bmp", planar_z);
	primitives.push_back(new Sphere(vec4(0, 0, 0, 1.0), 0.3, shader));

	return new Object(primitives);
}

// return: all the objects in the cornel box.
vector<Object*> star_objects() {
	vector<Object*> objects;
	objects.push_back(star_model());
	return objects;
}

// return: the lights in the cornel box.
vector<PointLight*> star_lights() {
	vector<PointLight*> lights;

	lights.push_back(new PointLight(vec3(18, 18, 18), vec4(0, -0.5, 0, 1.0)));

	return lights;
}

// return: a scene containing a star.
Scene star_scene() {
	return Scene(star_objects(), star_lights());
}

#endif // STAR_MODEL_H
