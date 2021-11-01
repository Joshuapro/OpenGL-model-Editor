#include"ebo.h"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#  include <windows.h>
#  undef max
#  undef min
#  undef DrawText
#endif

#ifndef __APPLE__
#  define GLEW_STATIC
#  include <GL/glew.h>
#endif

#ifdef __APPLE__
#   include <OpenGL/gl3.h>
#   define __gl_h_ /* Prevent inclusion of the old gl.h */ 
#else
#   ifdef _WIN32
#       include <windows.h>
#   endif
#   include <GL/gl.h>
#endif


// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO()
{
    ID = 0;
}

void EBO::update(std::vector<GLuint>& array)
{
    assert(ID != 0);
    // assert(!array.empty());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, array.size() * sizeof(GLuint), array.data(), GL_STATIC_DRAW);
}

// Binds the EBO

void EBO::init(){
    glGenBuffers(1, &ID);
}

void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::free()
{
	glDeleteBuffers(1,&ID);
}




