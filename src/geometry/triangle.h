#include <glm/glm.hpp>
#include "primitive.h"

using glm::vec3;
using glm::vec4;
using glm::normalize;
using glm::cross;
using glm::mat3;
using std::unique_ptr;

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
		Primitive(shader),
		v0(v0), v1(v1), v2(v2),
		e1(vec3(v1 - v0)),
		e2(vec3(v2 - v0)),
		normal(project_to_4D(normalize(cross(e2, e1))))
	{
	}

	virtual unique_ptr<vec4> intersection(const Ray &ray) const override {
        vec3 b = vec3(ray.start - v0);
        mat3 A = mat3(-vec3(ray.dir), e1, e2);
		vec3 intersection_in_plane_coordinates = inverse(A) * b;
		if(!is_inside(intersection_in_plane_coordinates)) return nullptr;
		else {
			vec4 intersection_in_world_coordinates = in_scene_coordinates(intersection_in_plane_coordinates);
        	return unique_ptr<vec4>(new vec4(intersection_in_world_coordinates));
		}
    }

    virtual vec4 compute_normal(vec4 point) const override {
        return normal;
    }

private:
	// return: whether the point [t u v], in the triangle's coordinate system
	//		   (i.e. e1 and e2 are the basis vectors), is inside the triangle
	bool is_inside(vec3 pos) const {
		float t = pos.x;
		float u = pos.y;
		float v = pos.z;

		return 0 < t && 0 <= u && 0 <= v && u + v <= 1;
	}

	// return: a point [t u v], in the triangle's coordinate system (i.e. e1
	//		   and e2 are the basis vectors) into the world coordinate system.
	vec4 in_scene_coordinates(vec3 point) const {
        float u = point.y;
        float v = point.z;
        return project_to_4D(vec3(v0) + u * e1 + v * e2);
	}
};

#endif // TRIANGLE_H
