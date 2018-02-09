#include <glm/glm.hpp>
#include <limits.h>
#include <vector>
#include "intersection.h"
#include "triangle.h"
#include "projection.h"

using glm::length;
using std::vector;

#ifndef SCENE_H
#define SCENE_H

// Contains all the geometry, lights, etc for a scene.
class Scene {
private:
    vector<Triangle> triangles;

public:
    Scene(vector<Triangle> triangles): triangles(triangles) {
    }

    // param ray:               A ray, in scene coordinates, check intersection with.
    // param excluded_triangle: the triangle to discount intersections with.
    //                          This can be useful to avoid self-intersection.
    // return:                  The closest intersection to the start of the ray.
    Intersection *closest_intersection(Ray &ray) {
        float closest_distance = std::numeric_limits<float>::max();
        int closest_triangle_idx = -1;
        vec4 intersection_pos;

        for (int i=0; i<this->triangles.size(); i++) {
            vec3 i_tri_cord = Intersection::with_triangle(triangles[i], ray);

            if (!triangles[i].is_inside(i_tri_cord)) {
                continue;
            }

            vec4 i_sce_cord = in_scene_coordinates(i_tri_cord, triangles[i]);

            // The distance from the point in scene coordinates to the ray in
            // scene coordinates.
            float dist = length(i_sce_cord - ray.start);

            if (dist < closest_distance) {
                closest_distance = dist;
                closest_triangle_idx = i;
                intersection_pos = i_sce_cord;
            }
        }

        if (closest_triangle_idx == -1) {
            return NULL;
        }

        return new Intersection(intersection_pos, triangles[closest_triangle_idx]);
    }

private:
    // return: a point [t u v], in the triangle's coordinate system (i.e. e1
	//		   and e2 are the basis vectors) into the world coordinate system.
	vec4 in_scene_coordinates(vec3 point, Triangle &tri) {
        float u = point.y;
        float v = point.z;
        return project_to_4D(vec3(tri.v0) + u * tri.e1 + v * tri.e2);
	}
};

#endif // SCENE_H
