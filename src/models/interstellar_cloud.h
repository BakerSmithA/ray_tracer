#include "../geometry/random.h"

#ifndef INTERSTELLAR_CLOUD_MODEL_H
#define INTERSTELLAR_CLOUD_MODEL_H

namespace interstellar_cloud {
    vec3 cloud_center() {
        return vec3(0.1f, 0.0f, -0.4f);
    }

    float cloud_radius() {
        return 0.6f;
    }

    Object *interstellar_cloud() {
    	Texture<vec3> *texture = new Stack3d("../texture_files/cloud_frames.bmp", 12);

    	const vec3 extinction_color = vec3(0.0f);
    	const float primary_step_size = 0.05f;
    	const float shadow_step_size = 0.07f;
        const float extinction_coefficient = 4.0f;
    	const float scattering_coefficient = 1.3f;

        Shader *shader = new Volumetric(texture,
    									extinction_color,
    									primary_step_size,
    									shadow_step_size,
    									extinction_coefficient,
    									scattering_coefficient);

        Primitive **primitives = new Primitive*[1];
        primitives[0] = new Sphere(project_to_4D(cloud_center()), cloud_radius(), shader);
        return new Object(1, primitives);
    }

    const Object **objects() {
        const Object **objects = new const Object*[3];
        objects[0] = interstellar_cloud();
        return objects;
    }

    vector<Light*> stars(int num_stars) {
        float radius = 0.01f;
        float dropoff = 7.5f;
        vec3 color = vec3(142, 135, 167) * vec3(1.0f/255.0f) * 1.0f;

        vector<Light*> lights;

        for (int i=0; i<num_stars; i++) {
            vec3 pos = random_in_sphere(cloud_center(), cloud_radius());
            Light *star = new PointLight(color, project_to_4D(pos), radius, dropoff);
            lights.push_back(star);
        }

        return lights;
    }

    Scene scene() {
        const int num_stars = 100;
        return Scene(1, objects(), stars(num_stars));
    }
}

#endif // INTERSTELLAR_CLOUD_MODEL_H
