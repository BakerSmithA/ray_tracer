#ifndef TRANSPARENCY_MODEL_H
#define TRANSPARENCY_MODEL_H

namespace transparency_demo {
    // return: the shader to be used for an object in the  box.
    Shader *surface_shader(vec3 col) {
    	//float mix_prop = 0.7f;
    	//Shader *material = Mix::ratio(new Mirror(), new Diffuse(col), mix_prop);
    	//return material;
        return new Diffuse(col);
    }

    // return: the floor of the  box.
    Object *floor() {
    	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
    	const Shader *shader = surface_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(C, B, A, shader));
    	triangles.push_back(new Triangle(C, D, B, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the left wall of the  box.
    Object *left_wall() {
    	const vec3 col = vec3(1.0f, 0.0f, 0.0f);
    	const Shader *shader = surface_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(A, E, C, shader));
    	triangles.push_back(new Triangle(C, E, G, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the right wall of the  box.
    Object *right_wall() {
    	const vec3 col = vec3(0.0f, 1.0f, 0.0f);
    	const Shader *shader = surface_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(F, B, D, shader));
    	triangles.push_back(new Triangle(H, F, D, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the back wall of the  box.
    Object *back_wall() {
    	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
    	const Shader *shader = surface_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(G, D, C, shader));
    	triangles.push_back(new Triangle(G, H, D, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the ceiling of the  box.
    Object *ceiling() {
    	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
    	const Shader *shader = surface_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(E, F, G, shader));
    	triangles.push_back(new Triangle(F, H, G, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: a lantern in which there will be a light.
    Object *lantern() {
        int octaves = 10;
        const Texture2d *noise = new Perlin2d(octaves, true);
        const Shader *mask = Projection::spherical(noise);

        const Shader *s1 = new Glass();
        const Shader *s2 = new Diffuse(vec3(1));

        Shader *shader = new Mask(s1, s2, mask);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0.0, 0.0, 0.0, 1.0), 0.3f, shader);
        return new Object(1, primitives);
    }

    // return: all the objects in the box.
    const Object **objects(int *num_objects) {
    	*num_objects = 6;
    	const Object **objects = new const Object*[6];

    	objects[0] = floor();
    	objects[1] = left_wall();
    	objects[2] = right_wall();
    	objects[3] = back_wall();
    	objects[4] = ceiling();
        objects[5] = lantern();

    	return objects;
    }

    // return: the lights in the box.
    vector<Light*> lights() {
    	vector<Light*> lights;

    	vec4 pos = vec4(0, 0, 0, 1.0);
    	vec3 col = vec3(18, 18, 18);
    	float radius = 0.01;

    	PointLight *light = new PointLight(col, pos, radius);
    	AmbientLight *ambient = new AmbientLight(vec3(0.2f, 0.2f, 0.2f));

    	lights.push_back(light);
    	lights.push_back(ambient);

    	return lights;
    }

    // return: the  box scene.
    Scene scene() {
    	int num_objects;
    	return Scene(num_objects, objects(&num_objects), lights());
    }
};

#endif // TRANSPARENCY_MODEL_H
