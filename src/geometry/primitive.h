#include <glm/glm.hpp>
#include <memory>
#include "projection.h"
#include "bounding_cube.h"
#include <optional>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::optional;
using std::array;

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

// Forward declarations.
class Shader;
class Object;

class Primitive {
public:
	// Used to tell the color of the triangle.
	const Shader *shader;
	// The smallest cube that encloses the primtitive.
	const BoundingCube bounding_cube;
	// The object that the primitive belongs to. This is set by the object
	// when the object is initialised.
	Object *parent_obj = NULL;

    Primitive(const Shader *shader, BoundingCube bounding_cube):
		shader(shader), bounding_cube(bounding_cube) {
	};

	// return: the position of the intersection with the primitive, or nullptr
	//		   if no intersection occurred.
	virtual optional<vec4> intersection(const Ray &ray) const = 0;

	// return: the normal to the primtive at the given point on the primitive.
    virtual vec4 normal_at(vec4 point) const = 0;
};

#endif // PRIMITIVE_H
