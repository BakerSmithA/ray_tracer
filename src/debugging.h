#pragma once

#include <glm/glm.hpp>
#include <iostream>

using glm::vec4;
using std::cout;
using std::endl;

void print_vec4(vec4 v) {
    cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << endl;
}

void print_mat4(mat4 m) {
    for (int i=0; i<4; i++) {
        print_vec4(m[i]);
    }
}
