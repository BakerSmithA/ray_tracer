#ifndef RAY_SPAWNER_H
#define RAY_SPAWNER_H

// Models a surface who's color is found by firing out another ray, e.g. mirror.
class RaySpawner: public Shader {
    // return: the ray direction used to find the color of the shader, e.g. the
    //         reflected ray for a mirror.
    virtual vec4 outgoing_ray_dir(const vec4 Position, const Primitive *prim, const Ray &incoming) const = 0;

    // return: the color of the shader, determined by shooting another ray into
    //         the scene. Or, black if the incoming ray cannot bounce anymore.
    vec3 color(vec4 position, const Primitive *prim, const Ray &incoming, const Scene &scene, const PointLight &light) const {
        if (!incoming.can_bounce()) {
            return vec3(0, 0, 0);
        }

        vec4 outgoing_dir = this->outgoing_ray_dir(position, prim, incoming);
        // The number of bounces is reduced due to this interaction.
        Ray outgoing_ray = Ray(position, outgoing_dir, incoming.bounces_remaining - 1);

        unique_ptr<Intersection> i = scene.closest_intersection(outgoing_ray, prim);
        if (i == nullptr) {
            return vec3(0, 0, 0);
        }

        return i->primitive->shader->shadowed_color(i->pos, i->primitive, outgoing_ray, scene, light);
    }
};

#endif // RAY_SPAWNER_H
