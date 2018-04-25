#pragma once

#include "../textures/ramp.h"
#include "../shaders/dist_from_center.h"
#include "../shaders/id.h"

namespace supernova_model {
    // return: shader which colors from red, to yellow, to blue, as the
    //         distance from the center of the cloud increases.
    Shader *inner_cloud_color(float radius) {
        vector<tuple<vec3, float>> colors;
        colors.push_back({ vec3(0,0,1), 0.0f });
        colors.push_back({ vec3(1,1,0.5), 0.7f });
        colors.push_back({ vec3(1,0,0), 1.0f });

        Texture<float> *ramp_tex = new Ramp(colors);
        return DistFromCenter::textured(ramp_tex, ramp_tex, radius);
    }

    // return: a shader for the cloud volume.
    Shader *inner_cloud_gray_volume() {
        const vec3 extinction_color = vec3(0.0f);
        const float primary_step_size = 0.05f;
        const float shadow_step_size = 0.07f;
        const float extinction_coefficient = 4.0f;
        const float scattering_coefficient = 1.3f;

        Texture<vec3> *texture = new Stack3d("../texture_files/cloud_frames.bmp", 12);
        return new Volumetric(texture, extinction_color, primary_step_size, shadow_step_size, extinction_coefficient, scattering_coefficient);
    }

    // return: a shader for a clouded cloud.
    Shader *inner_cloud_volume(float radius) {
        return Mix::multiply(inner_cloud_gray_volume(), inner_cloud_color(radius));
    }

    // return: a shader to act as a mask for the hole in the middle of the
    //         cloud where the particles have been blown away from the core.
    Shader *hole_mask(float radius) {
        vector<tuple<vec3, float>> colors;
        colors.push_back({ vec3(1), 0.0f });
        colors.push_back({ vec3(0), 1.0f });

        Texture<float> *ramp_tex = new Ramp(colors);
        return DistFromCenter::textured(ramp_tex, ramp_tex, radius);
    }

    Object *inner_cloud() {
        float radius = 0.6f;
        Shader *color_cloud = hole_mask(radius);//inner_cloud_volume(radius);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), radius, color_cloud);
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
