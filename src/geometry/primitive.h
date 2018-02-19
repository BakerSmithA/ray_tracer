#include <glm/glm.hpp>
#include <memory>
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
	// Used to indicate primitives that are part of the same object.
	const int obj_tag;

    Primitive(const Shader *shader, const int obj_tag): shader(shader), obj_tag(obj_tag) {
	};

	virtual unique_ptr<vec4> intersection(const Ray &ray) const = 0;
    virtual vec4 compute_normal(vec4 point) const = 0;
};

#endif // PRIMITIVE_H
