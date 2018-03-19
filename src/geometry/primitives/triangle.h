#include <glm/glm.hpp>
#include "primitive.h"
#include "../linear_alg.h"

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::cross;
using glm::mat3;
using std::optional;
using std::nullopt;

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle : public Primitive {
public:
    // The vertices of the triangle.
	const vec4 v0, v1, v2;

	// Vectors that define two edges of the triangle.
    // See equations (4) and (5) from Lab 1.
    const vec3 e1, e2;
    // Precomputed properties of the triangle.
    const vec4 normal;

	Triangle(vec4 v0, vec4 v1, vec4 v2, const Shader *shader):
		Primitive(shader, Triangle::make_bounding_cube(v0, v1, v2)),
		v0(v0), v1(v1), v2(v2),
		e1(vec3(v1 - v0)),
		e2(vec3(v2 - v0)),
		normal(project_to_4D(normalize(cross(e2, e1))))
	{
	}

	optional<vec4> intersection(const Ray &ray) const override {
        vec3 b = vec3(ray.start - v0);
        mat3 A = mat3(-vec3(ray.dir), e1, e2);
		//vec3 intersection_in_plane_coordinates = inverse(A) * b;
		vec3 intersection_in_plane_coordinates = solve_linear(A, b);

		if(!is_inside(intersection_in_plane_coordinates)) {
			return nullopt;
		}
        return in_scene_coordinates(intersection_in_plane_coordinates);
    }

    vec4 normal_at(vec4 point) const override {
        return normal;
    }

private:
	// return: whether the point [t u v], in the triangle's coordinate system
	//		   (i.e. e1 and e2 are the basis vectors), is inside the triangle
	bool is_inside(vec3 pos) const {
		float t = pos.x;
		float u = pos.y;
		float v = pos.z;

		return 0.0f < t && 0.0f <= u && 0.0f <= v && u + v <= 1.0f;
	}

	// return: a point [t u v], in the triangle's coordinate system (i.e. e1
	//		   and e2 are the basis vectors) into the world coordinate system.
	vec4 in_scene_coordinates(vec3 point) const {
        float u = point.y;
        float v = point.z;
        return project_to_4D(vec3(v0) + u * e1 + v * e2);
	}

	// return: the minimum and maximum corners of bounding cube around the
	//		   triangle.
	static BoundingCube make_bounding_cube(vec4 v0, vec4 v1, vec4 v2) {
		vec4 vertices[3] = { v0, v1, v2 };

		vec4 min = vertices[0];
		vec4 max = vertices[0];

		for (int i=1; i<3; i++) {
			if (vertices[i].x < min.x) min.x = vertices[i].x;
			if (vertices[i].y < min.y) min.y = vertices[i].y;
			if (vertices[i].z < min.z) min.z = vertices[i].z;
			if (vertices[i].x > max.x) max.x = vertices[i].x;
			if (vertices[i].y > max.y) max.y = vertices[i].y;
			if (vertices[i].z > max.z) max.z = vertices[i].z;
		}

		return BoundingCube(min, max);
	}
};

#endif // TRIANGLE_H
