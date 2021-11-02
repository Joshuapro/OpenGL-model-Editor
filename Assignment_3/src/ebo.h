#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glm/glm.hpp>  // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include<vector>

class EBO
{
public:
    typedef unsigned int GLuint;
    typedef int GLint;

	// ID reference of Elements Buffer Object
	GLuint ID;
    GLuint rows;
    GLuint cols;
    
	EBO();

    void init();

    void update(std::vector<GLuint>& indices);
	// Binds the EBO
	void bind();
	// Unbinds the EBO
	void free();

};

#endif
