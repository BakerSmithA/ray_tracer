#include "primitive.h"

using std::vector;

#ifndef OBJECT_H
#define OBJECT_H

// A collection of primitives.
class Object {
public:
    const vector<Primitive*> primitives;
    const BoundingCube bounding_cube;

    Object(const vector<Primitive*> primitives):
        primitives(primitives),
        bounding_cube(Object::make_bounding_cube(primitives))
    {
    }

private:
    // return: a bounding box around all the primtives.
    static BoundingCube make_bounding_cube(std::vector<Primitive*> primitives) {
		vec4 min = primitives[0]->bounding_cube.min;
		vec4 max = primitives[0]->bounding_cube.max;

		for (size_t i=1; i<primitives.size(); i++) {
			if (primitives[i]->bounding_cube.min.x < min.x) min.x = primitives[i]->bounding_cube.min.x;
			if (primitives[i]->bounding_cube.min.y < min.y) min.y = primitives[i]->bounding_cube.min.y;
			if (primitives[i]->bounding_cube.min.z < min.z) min.z = primitives[i]->bounding_cube.min.z;
			if (primitives[i]->bounding_cube.max.x > max.x) max.x = primitives[i]->bounding_cube.max.x;
			if (primitives[i]->bounding_cube.max.y > max.y) max.y = primitives[i]->bounding_cube.max.y;
			if (primitives[i]->bounding_cube.max.z > max.z) max.z = primitives[i]->bounding_cube.max.z;
		}

		return BoundingCube(min, max);
    }
};

#endif // OBJECT_H
