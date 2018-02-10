using std::runtime_error;

#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

// Models a constant light everywhere in the scene.
class AmbientLight: public Light {
public:
    // Inherit the default constructor.
    using Light::Light;

    // param point: the point to be illuminated.
    // param surface_normal: the normal of the object at the point.
    // return: the intensity of light at the position in the scene, i.e. the
    //         color of the light for ambient lights.
    vec3 intensity(vec4 point, vec4 surface_normal, Ray primary) const override {
        return this->color;
    }

    // return: whether the light casts shadows.
    virtual bool does_cast_shadows() const override {
        return false;
    }
};

#endif // AMBIENT_LIGHT_H
