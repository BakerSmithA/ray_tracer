#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

// return: a green sphere.
Object *sphere_object() {
	Primitive **primitives = new Primitive*[1];

	Shader *shader = new FlatColor(vec3(0.0, 1.0, 0.0));
	primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), 1.0, shader);

	return new Object(1, primitives);
}

// return: all the objects in the scene.
const Object **sphere_objects() {
	const Object **objects = new const Object*[1];
	objects[0] = sphere_object();
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
	return Scene(1, sphere_objects(), sphere_lights());
}

#endif // SPHERE_MODEL_H
