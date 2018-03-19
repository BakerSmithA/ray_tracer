#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

// return: a green sphere.
Object *sphere_object() {
	vector<Primitive*> primitives;

	Shader *shader = new FlatColor(vec3(0.0, 1.0, 0.0));
	primitives.push_back(new Sphere(vec4(0, 0, 0, 1.0), 1.0, shader));

	return new Object(primitives);
}

// return: all the objects in the scene.
vector<Object*> sphere_objects() {
	vector<Object*> objects;
	objects.push_back(sphere_object());
	return objects;
}

// return: the lights in the sphere scene.
vector<Light*> sphere_lights() {
	vector<Light*> lights;

	AmbientLight *ambient = new AmbientLight(vec3(1.0, 1.0, 1.0));
	lights.push_back(ambient);

	return lights;
}

// return: a scene containing a sphere.
Scene sphere_scene() {
	return Scene(sphere_objects(), sphere_lights());
}

#endif // SPHERE_MODEL_H
