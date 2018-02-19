#include "intersection.h"
#include "primitive.h"

using std::vector;

#ifndef OBJECT_H
#define OBJECT_H

// A collection of primtives to make a single object.
class Object {
public:
    const vector<Primitive*> primitives;

    Object(vector<Primitive*> primitives): primitives(primitives) {
    }

    unique_ptr<Intersection> intersection() {
        return nullptr;
    }
};

#endif // OBJECT_H
