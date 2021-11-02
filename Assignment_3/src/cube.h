#ifndef Cube_CLASS_H
#define Cube_CLASS_H

#include <glm/glm.hpp>  // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include<vector>

class Cube{

public:
    typedef unsigned int GLuint;
    typedef int GLint;
    int mode;
    std::vector<glm::vec3> pos = {
        glm::vec3(-0.5,0.5,-0.5),
        glm::vec3(-0.5,0.5,0.5),
        glm::vec3(0.5,0.5,-0.5),
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(-0.5,-0.5,-0.5),
        glm::vec3(-0.5,-0.5,0.5),
        glm::vec3(0.5,-0.5,-0.5),
        glm::vec3(0.5,-0.5,0.5),
    };

    std::vector<GLuint> ind = {
        0,1,2,
        1,2,3,

        4,5,6,
        5,6,7,

        0,1,5,
        0,4,5,

        2,3,7,
        2,6,7,

        0,2,6,
        0,4,6,
        
        1,5,7,
        1,3,7
    };

    

};

#endif
