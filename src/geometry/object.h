#pragma once

#include "primitives/primitive.h"
#include <iostream>
#include "../debugging.h"

using std::vector;

// A collection of primitives.
//
// WARNING: when the object is destroyed, the primitives it contains will also
// be destroyed.
class Object {
public:
    const int num_prims;
    Primitive **primitives;
    const BoundingCube bounding_cube;

    Object(const int num_prims, Primitive **primitives):
        num_prims(num_prims),
        primitives(primitives),
        bounding_cube(Object::make_bounding_cube(num_prims, primitives))
    {
        for (int i=0; i<num_prims; i++) {
            this->primitives[i]->parent_obj = this;
        }
    }

    ~Object() {
        for (int i=0; i<num_prims; i++) {
            delete this->primitives[i];
        }

        delete[] this->primitives;
    }

    // return: the center point of the object in world coordinates.
    vec4 center() const {
        return bounding_cube.center;
    }

    // return: the world_point converted to in the axis aligned coordinate
    //         space as of this object, as defined by the bounding box.
    //         The returned vector in therefore in the bounds 0-1.
    vec4 converted_world_to_obj(vec4 world_point) const {
        vec4 min = this->bounding_cube.min;
        vec4 max = this->bounding_cube.max;

        // Multipliers to convert from world coordinates to object coordinates.
        float mx = 1 / (max.x - min.x);
        float my = 1 / (max.y - min.y);
        float mz = 1 / (max.z - min.z);

        vec4 scale = vec4(mx, my, mz, 1.0f);

        return (world_point - this->bounding_cube.min) * scale;
    }

private:
    // return: a bounding box around all the primtives.
    static BoundingCube make_bounding_cube(const int num_prims, Primitive **primitives) {
        // Initialise the min and max, these will be updated below.
		vec4 min = primitives[0]->bounding_cube.min;
		vec4 max = primitives[0]->bounding_cube.max;

		for (int i=1; i<num_prims; i++) {
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
