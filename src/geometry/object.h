#include "primitive.h"

using std::vector;

#ifndef OBJECT_H
#define OBJECT_H

// A collection of primitives.
class Object {
public:
    const vector<Primitive*> primitives;

    Object(const vector<Primitive*> primitives): primitives(primitives) {
    }
};

#endif // OBJECT_H
