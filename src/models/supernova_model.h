#pragma once

#include "../textures/ramp.h"
#include "../shaders/dist_from_center.h"

namespace supernova_model {
    Object *inner_cloud() {
        float radius = 0.6f;

        vector<tuple<vec3, float>> colors;
        colors.push_back({ vec3(0,0,1), 0.0f });
        colors.push_back({ vec3(1,1,0.5), 0.7f });
        colors.push_back({ vec3(1,0,0), 1.0f });

        Texture<float> *ramp_tex = new Ramp(colors);
        Shader *ramp = DistFromCenter::textured(ramp_tex, ramp_tex, radius);

        const vec3 extinction_color = vec3(0.0f);
        // const float primary_step_size = 0.015f;
        // const float shadow_step_size = 0.03f;
        const float primary_step_size = 0.05f;
        const float shadow_step_size = 0.07f;
        const float extinction_coefficient = 4.0f;
        const float scattering_coefficient = 1.3f;

        Texture<vec3> *texture = new Stack3d("../texture_files/cloud_frames.bmp", 12);
        Shader *cloud = new Volumetric(texture,
                                       extinction_color,
                                       primary_step_size,
                                       shadow_step_size,
                                       extinction_coefficient,
                                       scattering_coefficient);

        Shader *combined = Mix::multiply(cloud, ramp);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), radius, combined);
        return new Object(1, primitives);
    }

    const Object **objects() {
        const Object **objects = new const Object*[1];
        objects[0] = inner_cloud();
        return objects;
    }

    // return: the lights in the cornel box.
    vector<Light*> lights() {
    	vector<Light*> lights;

    	vec4 light1_pos = vec4(0, 0, 0, 1.0);
    	vec3 light1_col = vec3(25.0f);
    	PointLight *light1 = new PointLight(light1_col, light1_pos, 0.001f, 0.25f);

    	lights.push_back(light1);
    	return lights;
    }

    Scene scene() {
    	return Scene(1, objects(), lights());
    }
}
