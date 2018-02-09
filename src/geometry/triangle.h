#include <glm/glm.hpp>
#include "projection.h"

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::cross;
using glm::epsilonEqual;

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle {
public:
    // The vertices of the triangle.
	const vec4 v0, v1, v2;
	const vec3 color;
	// Vectors that define two edges of the triangle.
    // See equations (4) and (5) from Lab 1.
    const vec3 e1, e2;
    // Precomputed properties of the triangle.
    const vec4 normal;

	Triangle(vec4 v0, vec4 v1, vec4 v2, vec3 color):
		v0(v0), v1(v1), v2(v2),
		color(color),
		e1(vec3(v1 - v0)),
		e2(vec3(v2 - v0)),
		normal(project_to_4D(normalize(cross(e2, e1))))
	{
	}

	// return: whether the point [t u v], in the triangle's coordinates system
	//		   (i.e. e1 and e2 are the basis vectors), is inside the triangle
	bool is_inside(vec3 pos) {
		float t = x.x;
		float u = x.y;
		float v = x.z;

		return 0 < t && 0 <= u && 0 <= v && u + v <= 1;
	}

	bool operator == (const Triangle &other) const {
		return epsilonEqual(this->color, other->color, 0.0001)
			&& epsilonEqual(this->v0, other->v0, 0.0001)
			&& epsilonEqual(this->v1, other->v1, 0.0001)
			&& epsilonEqual(this->v2, other->v2, 0.0001);
 	}
};

#endif // TRIANGLE_H
