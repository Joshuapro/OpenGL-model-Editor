#ifndef Cube_CLASS_H
#define Cube_CLASS_H

#include <glm/glm.hpp>  // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include<vector>
#include<iostream>

class Cube{

public:
    typedef unsigned int GLuint;
    typedef int GLint;
    int mode;
    int posstart;
    int poscount = 0;
    int indstart;
    int indcount = 0; 
    

    // Cube(int start, int end){

    // }

    Cube(std::vector<glm::vec3>& posvec, std::vector<GLuint>& indvec){
        
        posstart = posvec.size();
        for (int i = 0; i < pos.size(); i++){
            poscount += 1;
            posvec.push_back(pos[i]);
        }

        indstart = indvec.size();
        for (int i = 0; i < ind.size(); i++){
            indcount += 1;
            indvec.push_back(ind[i] + posstart);
        }
        
    }

    
    std::vector<glm::vec3> pos = {
        glm::vec3(-0.1,0.1,-0.1),
        glm::vec3(-0.1,0.1,0.1),
        glm::vec3(0.1,0.1,-0.1),
        glm::vec3(0.1,0.1,0.1),
        glm::vec3(-0.1,-0.1,-0.1),
        glm::vec3(-0.1,-0.1,0.1),
        glm::vec3(0.1,-0.1,-0.1),
        glm::vec3(0.1,-0.1,0.1),
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
