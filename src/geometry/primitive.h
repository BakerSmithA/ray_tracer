#include <glm/glm.hpp>
#include "projection.h"

using glm::vec3;
using glm::vec4;
using std::unique_ptr;

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

// Forward declaration of the shader class;
class Shader;

class Primitive {
public:

	// Used to tell the color of the triangle.
	const Shader *shader;

    Primitive(const Shader *shader) : shader(shader) {};

	virtual unique_ptr<vec4> intersection(const Ray &ray) const = 0;
    virtual vec4 compute_normal(vec4 point) const = 0;
};

#endif // PRIMITIVE_H
