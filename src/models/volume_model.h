#include "../textures/uniform_3d.h"
#include "../textures/stacked_3d.h"
#include "../shaders/volumetric.h"

#ifndef VOLUME_MODEL_H
#define VOLUME_MODEL_H

// return: the triangles scaled to fit in a -1 to 1 box, and flips the normals.
Primitive **scaled_flipped_triangles(vector<Triangle*> triangles) {
	Primitive **new_triangles = new Primitive*[triangles.size()];

	for(size_t i=0; i<triangles.size(); i++) {
		Triangle* triangle = static_cast<Triangle*>(triangles[i]);
		vec4 new_v0 = triangle->v0;
		vec4 new_v1 = triangle->v1;
		vec4 new_v2 = triangle->v2;

		new_v0 *= 2/L;
		new_v1 *= 2/L;
		new_v2 *= 2/L;

		new_v0 -= vec4(1,1,1,1);
		new_v1 -= vec4(1,1,1,1);
		new_v2 -= vec4(1,1,1,1);

		new_v0.x *= -1;
		new_v1.x *= -1;
		new_v2.x *= -1;

		new_v0.y *= -1;
		new_v1.y *= -1;
		new_v2.y *= -1;

		new_v0.w = 1.0;
		new_v1.w = 1.0;
		new_v2.w = 1.0;

		new_triangles[i] = new Triangle(new_v1, new_v0, new_v2, triangle->shader);
        //new_triangles[i] = new Triangle(new_v0, new_v1, new_v2, triangle->shader);
	}

	return new_triangles;
}

Object *volume_cube_model() {
    Shader *shader = new Diffuse(vec3(1,0,0));

    vector<Triangle*> triangles;
    // floor
    triangles.push_back(new Triangle(C, B, A, shader));
    triangles.push_back(new Triangle(C, D, B, shader));

    // left
    triangles.push_back(new Triangle(A, E, C, shader));
    triangles.push_back(new Triangle(C, E, G, shader));

    // right
    triangles.push_back(new Triangle(F, B, D, shader));
    triangles.push_back(new Triangle(H, F, D, shader));

    // back
    triangles.push_back(new Triangle(G, D, C, shader));
    triangles.push_back(new Triangle(G, H, D, shader));

    // ceiling
    triangles.push_back(new Triangle(E, F, G, shader));
	triangles.push_back(new Triangle(F, H, G, shader));

    // front
    G.z = 0;
    D.z = 0;
    C.z = 0;
    H.z = 0;
    triangles.push_back(new Triangle(D, G, C, shader));
    triangles.push_back(new Triangle(H, G, D, shader));
    // triangles.push_back(new Triangle(G, D, C, shader));
    // triangles.push_back(new Triangle(G, H, D, shader));

    return new Object(triangles.size(), scaled_flipped_triangles(triangles));
}

// return: all the objects in the cornel box.
const Object **volume_objects() {
	const Object **objects = new const Object*[1];
    objects[0] = volume_cube_model();
	return objects;
}

// return: the lights in the cornel box.
vector<Light*> volume_lights() {
	vector<Light*> lights;

	vec4 pos = vec4(0, -1.2, -1.2, 1.0);
	vec3 col = vec3(18, 18, 18);
	float radius = 0.01;

	PointLight *light = new PointLight(col, pos, radius);
    AmbientLight *ambient = new AmbientLight(vec3(0.3,0.3,0.3));

	lights.push_back(light);
    lights.push_back(ambient);

	return lights;
}

// return: a scene containing a star.
Scene volume_scene() {
	return Scene(1, volume_objects(), volume_lights());
}

#endif // VOLUME_MODEL_H
