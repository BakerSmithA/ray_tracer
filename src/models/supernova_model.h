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
        return DistFromCenter::textured(ramp_tex, 0.0f, radius);
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
    Shader *inner_cloud_hole_mask(float radius) {
        vector<tuple<vec3, float>> colors;
        colors.push_back({ vec3(1), 0.0f });
        colors.push_back({ vec3(1), 0.25f });
        colors.push_back({ vec3(0), 1.0f });

        Texture<float> *ramp_tex = new Ramp(colors);
        return DistFromCenter::textured(ramp_tex, ramp_tex, radius);
    }

    // return: a cloud shader which changes color as the distance to the center
    //         increases, and has a hole in the middle.
    Shader *inner_cloud_shader(float radius) {
        Shader *id = new Id();
        Shader *volume = inner_cloud_volume(radius);
        Shader *mask = inner_cloud_hole_mask(radius);

        return new Mask(id, volume, mask);
    }

    Object *inner_cloud() {
        float radius = 0.6f;
        Shader *shader = inner_cloud_shader(radius);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), radius, shader);
        return new Object(1, primitives);
    }

    // return: shader which colors from red, to yellow, to blue, as the
    //         distance from the center of the cloud increases.
    Shader *outer_cloud_color(float mask_start, float radius) {
        vector<tuple<vec3, float>> colors;
        colors.push_back({ vec3(0,0,0), 0.0f });
        colors.push_back({ vec3(1,1,0.7), mask_start });
        colors.push_back({ vec3(1,0,0), 1.0f });

        Texture<float> *ramp_tex = new Ramp(colors);
        return DistFromCenter::textured(ramp_tex, 1.0f, radius);
    }

    // return: a shader for the cloud volume.
    Shader *outer_cloud_gray_volume() {
        const vec3 extinction_color = vec3(0.0f);
        // const float primary_step_size = 0.05f;
        // const float shadow_step_size = 0.07f;
        const float primary_step_size = 0.015f;
        const float shadow_step_size = 0.03f;
        const float extinction_coefficient = 7.5f;
        const float scattering_coefficient = 1.0f;
        const int octaves = 6;

        Texture<vec3> *texture = new Perlin<vec3>(octaves);
        Texture<vec3> *thresholded = new Threshold<vec3>(texture, 0.37);
        return new Volumetric(thresholded, extinction_color, primary_step_size, shadow_step_size, extinction_coefficient, scattering_coefficient);
    }

    // return: a shader for a clouded cloud.
    Shader *outer_cloud_volume(float mask_start, float radius) {
        return Mix::multiply(outer_cloud_gray_volume(), outer_cloud_color(mask_start, radius));
    }

    // return: a shader to act as a mask for the hole in the middle of the
    //         cloud where the particles have been blown away from the core.
    Shader *outer_cloud_hole_mask(float mask_start, float radius) {
        vector<tuple<vec3, float>> colors;
        colors.push_back({ vec3(1), 0.0f });
        colors.push_back({ vec3(1), mask_start });
        colors.push_back({ vec3(0), 1.0f });

        Texture<float> *ramp_tex = new Ramp(colors);
        return DistFromCenter::textured(ramp_tex, ramp_tex, radius);
    }

    // return: a cloud shader which changes color as the distance to the center
    //         increases, and has a hole in the middle.
    Shader *outer_cloud_shader(float mask_start, float radius) {
        Shader *id = new Id();
        Shader *volume = outer_cloud_volume(mask_start, radius);
        Shader *mask = outer_cloud_hole_mask(mask_start, radius);

        return new Mask(id, volume, mask);
    }

    Object *outer_cloud() {
        float radius = 1.0f;
        float inner_radius = 0.9f;
        Shader *shader = outer_cloud_shader(inner_radius, radius);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(vec4(0, 0, 0, 1.0), radius, shader);
        return new Object(1, primitives);
    }

    const Object **objects() {
        const Object **objects = new const Object*[2];
        objects[0] = inner_cloud();
        objects[1] = outer_cloud();
        return objects;
    }

    // return: the lights in the cornel box.
    vector<Light*> lights() {
    	vector<Light*> lights;

    	vec4 light1_pos = vec4(0, 0, 0, 1.0);
    	vec3 light1_col = vec3(100.0f);
    	PointLight *light1 = new PointLight(light1_col, light1_pos, 0.001f, 0.5f);

    	lights.push_back(light1);
    	return lights;
    }

    Scene scene() {
    	return Scene(2, objects(), lights());
    }
}
