#pragma once

namespace dirt_model {
    // return: the shader to be used for an object in the cornel box.
    Shader *cornel_shader(vec3 col) {
    	float mix_prop = 0.7f;
    	Shader *material = Mix::ratio(new Mirror(), new Diffuse(col), mix_prop);
    	return material;
    }

    // return: the floor of the cornel box.
    Object *cornel_floor() {
    	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
    	const Shader *shader = cornel_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(C, B, A, shader));
    	triangles.push_back(new Triangle(C, D, B, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the left wall of the cornel box.
    Object *cornel_left_wall() {
    	const vec3 col = vec3(1.0f, 0.0f, 0.0f);
    	const Shader *shader = cornel_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(A, E, C, shader));
    	triangles.push_back(new Triangle(C, E, G, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the right wall of the cornel box.
    Object *cornel_right_wall() {
    	const vec3 col = vec3(0.0f, 1.0f, 0.0f);
    	const Shader *shader = cornel_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(F, B, D, shader));
    	triangles.push_back(new Triangle(H, F, D, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the back wall of the cornel box.
    Object *cornel_back_wall() {
    	const vec3 col = vec3(1.0f, 1.0f, 1.0f);
    	const Shader *diffuse_shader = cornel_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(G, D, C, diffuse_shader));
    	triangles.push_back(new Triangle(G, H, D, diffuse_shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the ceiling of the cornel box.
    Object *cornel_ceiling() {
    	const vec3 col = vec3(1.0f, 1.0f, 1.0f);//vec3(0.15f, 0.75f, 0.75f);
    	const Shader *shader = cornel_shader(col);

    	vector<Triangle*> triangles;
    	triangles.push_back(new Triangle(E, F, G, shader));
    	triangles.push_back(new Triangle(F, H, G, shader));

    	return new Object(triangles.size(), scaled_triangles(triangles));
    }

    // return: the short (red) block in the cornel box.
    Object *cornel_short_block() {
    	vec4 A = vec4(290,0,114,1);
    	vec4 B = vec4(130,0, 65,1);
    	vec4 C = vec4(240,0,272,1);
    	vec4 D = vec4(82,0,225,1);

    	vec4 E = vec4(290,165,114,1);
    	vec4 F = vec4(130,165, 65,1);
    	vec4 G = vec4(240,165,272,1);
    	vec4 H = vec4(82,165,225,1);

    	const vec3 col = vec3(0.75f, 0.15f, 0.15f);
    	const Shader *shader = cornel_shader(col);

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

    // return: the tall (blue) block in the cornel box.
    Object *cornel_tall_block() {
    	vec4 A = vec4(423,0,247,1);
    	vec4 B = vec4(265,0,296,1);
    	vec4 C = vec4(472,0,406,1);
    	vec4 D = vec4(314,0,456,1);

    	vec4 E = vec4(423,330,247,1);
    	vec4 F = vec4(265,330,296,1);
    	vec4 G = vec4(472,330,406,1);
    	vec4 H = vec4(314,330,456,1);

    	const vec3 col = vec3(0.15f, 0.15f, 0.75f);
    	const Shader *shader = cornel_shader(col);

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

    // return: a glass sphere.
    Object *cornel_large_sphere() {
        int octaves = 8;
    	const Texture<vec2> *noise = new Perlin<vec2>(octaves);
    	const Shader *mask = Projection::planar(noise, planar_z);

    	const Shader *s1 = new Diffuse(vec3(0));
    	const Shader *s2 = new Mirror();

    	Shader *shader = new Mask(s1, s2, mask);

    	Primitive **primitives = new Primitive*[1];
    	primitives[0] = new Sphere(vec4(0.0, 0.3, 0, 1.0), 0.3, shader);
    	return new Object(1, primitives);
    }

    // return: all the objects in the cornel box.
    const Object **cornel_objects(int *num_objects) {
    	*num_objects = 6;
    	const Object **objects = new const Object*[6];

    	objects[0] = cornel_floor();
    	objects[1] = cornel_left_wall();
    	objects[2] = cornel_right_wall();
    	objects[3] = cornel_back_wall();
    	objects[4] = cornel_ceiling();
    	objects[5] = cornel_large_sphere();

    	return objects;
    }

    // return: the lights in the cornel box.
    vector<Light*> cornel_lights() {
    	vector<Light*> lights;

    	vec4 pos = vec4(0, -0.5, -0.7, 1.0);
    	// vec3 col = vec3(18, 18, 18);
    	vec3 col = vec3(18, 18, 18);
    	float radius = 0.1;

    	PointLight *light = new PointLight(col, pos, radius);
    	// AmbientLight *ambient = new AmbientLight(vec3(0.15f, 0.15f, 0.15f));
    	AmbientLight *ambient = new AmbientLight(vec3(0.2f, 0.2f, 0.2f));

    	lights.push_back(light);
    	lights.push_back(ambient);

    	return lights;
    }

    // return: the cornel box scene.
    Scene scene() {
    	int num_objects;
    	return Scene(num_objects, cornel_objects(&num_objects), cornel_lights());
    }
}
