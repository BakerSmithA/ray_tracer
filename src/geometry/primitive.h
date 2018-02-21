#include <glm/glm.hpp>
#include <memory>
#include "projection.h"
#include "bounding_cube.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::unique_ptr;
using std::array;

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

// Forward declaration of the shader class;
class Shader;

class Primitive {
public:
	// Used to tell the color of the triangle.
	const Shader *shader;
	// The smallest cube that encloses the primtitive.
	const BoundingCube bounding_cube;
	// Used to indicate primitives that are part of the same object.
	const int obj_tag;

    Primitive(const Shader *shader, BoundingCube bounding_cube, const int obj_tag):
		shader(shader), bounding_cube(bounding_cube), obj_tag(obj_tag) {
	};

	// return: whether this primitive and the given primitive belong to the
	//		   same object.
	bool is_same_object(const Primitive *prim) const {
		return this->obj_tag == prim->obj_tag;
	}

	// return: the position of the intersection with the primitive, or nullptr
	//		   if no intersection occurred.
	virtual unique_ptr<vec4> intersection(const Ray &ray) const = 0;

	// return: the normal to the primtive at the given point on the primitive.
    virtual vec4 compute_normal(vec4 point) const = 0;
};

#endif // PRIMITIVE_H
