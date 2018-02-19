#ifndef SMOKE_H
#define SMOKE_H

// Models suspended particles.
class Smoke: public Shader {
    // return: a color depending on how far the ray has to travel before
    //         exiting the smoke.
    vec3 color(const vec4 position, const Primitive *primitive, const Ray &incoming, const Scene &scene, const Light &light) const override {
        return vec3(0,0,0);
    }
};

#endif
