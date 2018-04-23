#pragma once

namespace procedural_volume {
    Object *volume() {
        //Texture<vec3> *texture = new Stack3d("../texture_files/cloud_frames.bmp", 12);
        int octaves = 7;
        Texture<vec3> *texture = new Perlin<vec3>(octaves);
        Texture<vec3> *thresholded = new Threshold<vec3>(texture, 0.5);

    	const vec3 extinction_color = vec3(0.0f);
        // const float primary_step_size = 0.015f;
    	// const float shadow_step_size = 0.02f;
        const float primary_step_size = 0.015f;
        const float shadow_step_size = 0.07f;
        const float extinction_coefficient = 5.5f;
    	const float scattering_coefficient = 2.0f;

        Shader *shader = new Volumetric(thresholded,
    									extinction_color,
    									primary_step_size,
    									shadow_step_size,
    									extinction_coefficient,
    									scattering_coefficient);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0.1, 0, -0.4, 1.0), 0.6, shader);
        return new Object(1, primitives);
    }

    const Object **objects() {
        const Object **objects = new const Object*[1];
        objects[0] = volume();
        return objects;
    }

    // return: the lights in the cornel box.
    vector<Light*> lights() {
    	vector<Light*> lights;

    	vec4 light1_pos = vec4(1.25, -2.0, 1.0, 1.0);
    	vec3 light1_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 35.0f;
        // 
    	// vec4 light2_pos = vec4(0.0, 1.0, 0.0, 1.0);
    	// vec3 light2_col = vec3(142, 135, 167) * vec3(1.0f/255.0f) * 1.15f;
        //
    	// vec3 ambient_col = vec3(211.f, 175.0f, 176.0f) * vec3(1.0f/255.0f) * 0.03f;

    	PointLight *light1 = new PointLight(light1_col, light1_pos, 0.05f, 0.25f);
    	// PointLight *light2 = new PointLight(light2_col, light2_pos, 0.4f, 0.25f);
        // AmbientLight *ambient = new AmbientLight(ambient_col);

    	lights.push_back(light1);
    	//lights.push_back(light2);
        //lights.push_back(ambient);

    	return lights;
    }

    Scene scene() {
    	return Scene(1, objects(), lights());
    }
}
