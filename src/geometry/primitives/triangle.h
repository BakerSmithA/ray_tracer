#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "primitive.h"

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
		vec3 intersection_in_plane_coordinates = this->solve(A, b);

		if(!is_inside(intersection_in_plane_coordinates)) {
			return nullopt;
		}
        return in_scene_coordinates(intersection_in_plane_coordinates);
    }

    vec4 normal_at(vec4 point) const override {
        return normal;
    }

private:
	// return: the solution Ax=b using Cramer's rule, described
	//		   https://rosettacode.org/wiki/Cramer%27s_rule
	vec3 solve(mat3 A, vec3 b) const {
		const float *source_A = (const float*)glm::value_ptr(A);
		const float *source_b = (const float*)glm::value_ptr(b);

		float inv_det_A = 1.0f / this->determinant(source_A);

		float x1 = this->determinant0(source_A, source_b) * inv_det_A;
		float x2 = this->determinant1(source_A, source_b) * inv_det_A;
		float x3 = this->determinant2(source_A, source_b) * inv_det_A;


		// mat3 x1_mat = mat3(A[0], A[1], b);
		// printf("%f == %f\n", this->determinant2(source_A, source_b), glm::determinant(x1_mat));

		return vec3(x1, x2, x3);
	}

	// return: the determinant of the the matrix A
	//__attribute__((always_inline))
	float determinant(const float source[16]) const {
		// Column vec 0, i.e. b
		float a = source[0];
		float b = source[1];
		float c = source[2];

		// Column vec 1, i.e. A[1]
		float d = source[3];
		float e = source[4];
		float f = source[5];

		// Column vec 1, i.e. A[2]
		float g = source[6];
		float h = source[7];
		float i = source[8];

		return a * (e*i - f*h)
			 - b * (d*i - f*g)
			 + c * (d*h - e*g);
	}

	// return: the determinant of the the matrix [ b, A[1], A[2] ]
	__attribute__((always_inline))
	float determinant0(const float source[16], const float source_b[3]) const {
		// Column vec 0, i.e. b
		float a = source_b[0];
		float b = source_b[1];
		float c = source_b[2];

		// Column vec 1, i.e. A[1]
		float d = source[3];
		float e = source[4];
		float f = source[5];

		// Column vec 1, i.e. A[2]
		float g = source[6];
		float h = source[7];
		float i = source[8];

		return a * (e*i - f*h)
			 - b * (d*i - f*g)
			 + c * (d*h - e*g);
	}

	// return: the determinant of the the matrix [ A[0], b, A[2] ]
	__attribute__((always_inline))
	float determinant1(const float source[16], const float source_b[3]) const {
		// Column vec 0, i.e. A[0]
		float a = source[0];
		float b = source[1];
		float c = source[2];

		// Column vec 1, i.e. b
		float d = source_b[0];
		float e = source_b[1];
		float f = source_b[2];

		// Column vec 2, i.e. A[2]
		float g = source[6];
		float h = source[7];
		float i = source[8];

		return a * (e*i - f*h)
			 - b * (d*i - f*g)
			 + c * (d*h - e*g);
	}

	// return: the determinant of the the matrix [ A[0], A[1], b ]
	__attribute__((always_inline))
	float determinant2(const float source[16], const float source_b[3]) const {
		// Column vec 0, i.e. A[0]
		float a = source[0];
		float b = source[1];
		float c = source[2];

		// Column vec 1, i.e. A[1]
		float d = source[3];
		float e = source[4];
		float f = source[5];

		// Column vec 2, i.e. b
		float g = source_b[0];
		float h = source_b[1];
		float i = source_b[2];

		return a * (e*i - f*h)
			 - b * (d*i - f*g)
			 + c * (d*h - e*g);
	}

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
