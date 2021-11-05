#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>  // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4


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

// From: https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#endif

class VertexArrayObject
{
public:
    unsigned int id;

    VertexArrayObject() : id(0) {}

    // Create a new VAO
    void init();

    // Select this VAO for subsequent draw calls
    void bind();

    // Release the id
    void free();
};

class VertexBufferObject
{
public:
    typedef unsigned int GLuint;
    typedef int GLint;

    GLuint id;
    GLuint rows;
    GLuint cols;

    VertexBufferObject() : id(0), rows(0), cols(0) {}

    // Create a new empty VBO
    void init();

    // Updates the VBO
    template<typename T>
    void update(const std::vector<T>& array)
    {
      assert(id != 0);
      // assert(!array.empty()); 
      glBindBuffer(GL_ARRAY_BUFFER, id);
      glBufferData(GL_ARRAY_BUFFER, sizeof(T) * array.size(), array.data(), GL_DYNAMIC_DRAW);

      cols = array.size();
	  if(cols > 0)
      rows = array[0].length();
      check_gl_error();
    };
	template<typename T>
	void updateN(const std::vector<T>& array)
	{
		assert(id != 0);
		// assert(!array.empty()); 
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * array.size(), array.data(), GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		check_gl_error();
	};
    // Select this VBO for subsequent draw calls
    void bind();

    // Release the id
    void free();
};

// This class wraps an OpenGL program composed of two shaders
class Program
{
public:
  typedef unsigned int GLuint;
  typedef int GLint;

  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint program_shader;

  Program() : vertex_shader(0), fragment_shader(0), program_shader(0) { }

  // Create a new shader from the specified source strings
  bool init(const std::string &vertex_shader_string,
  const std::string &fragment_shader_string,
  const std::string &fragment_data_name);

  // Select this shader for subsequent draw calls
  void bind();

  // Release all OpenGL objects
  void free();

  // Return the OpenGL handle of a named shader attribute (-1 if it does not exist)
  GLint attrib(const std::string &name) const;

  // Return the OpenGL handle of a uniform attribute (-1 if it does not exist)
  GLint uniform(const std::string &name) const;

  // Bind a per-vertex array attribute
  GLint bindVertexAttribArray(const std::string &name, VertexBufferObject& VBO) const;

  GLuint create_shader_helper(GLint type, const std::string &shader_string);

};


class Bunny{
public:
    typedef unsigned int GLuint;
    typedef int GLint;
    int posstart;
    int poscount = 0;
    int indstart;
    int indcount = 0;
    float baryy = 0;
    float baryx = 0;
    float baryz = 0;
	glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f);
	std::vector<glm::vec3> pos;
	glm::vec3 modelpos;
	float angleY;
	glm::mat4 model;
	int shading_mode = 1;
	Bunny() {
		model  = glm::mat4(1.0f);
		modelpos = glm::vec3(0.0f);
	}
    std::vector<GLuint> ind;

	std::vector<glm::vec3> posvec;
	std::vector<GLuint> indvec;

    Bunny(std::string url);
	std::vector<glm::vec3> norms;
	std::vector<glm::vec3> phongnorms;
	glm::vec3 triangleNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{

		glm::vec3 u = a - b;
		glm::vec3 v = b - c;
		return glm::cross(u,v);
	}
	void calcNormal() {
		
		norms.clear();
		norms.resize(pos.size());
		std::vector<int> counters;
		counters.resize(pos.size());
		phongnorms.resize(norms.size());
		for (int i = 0; i < ind.size(); i+=3) {
			int i0 = ind[i];
			int i1 = ind[i+1];
			int i2 = ind[i+2];
		 
			glm::vec3 pos0 = pos.at(i0);
			glm::vec3 pos1 = pos.at(i1);
			glm::vec3 pos2 = pos.at(i2);
			glm::vec3 N = triangleNormal(pos0, pos1, pos2);
			N = glm::normalize(N);
			norms[i0] = N;
			norms[i1] = N;
			norms[i2] = N;
			phongnorms[i0] += N;
			phongnorms[i1] += N;
			phongnorms[i2] += N;

			counters[i0]++;
			counters[i1]++;
			counters[i2]++;
		}
		
		for (int i = 0; i < static_cast<int>(norms.size()); ++i) {
			//phongnorms[i] = norms[i];
			if (counters[i] > 0) {

				phongnorms[i] /= counters[i];
			}
			else {
				glm::normalize(phongnorms[i]);

			}

		}
	}
    void pushVec();
};



class Bumpy{
public:
    typedef unsigned int GLuint;
    typedef int GLint;
    int posstart;
    int poscount = 0;
    int indstart;
    int indcount = 0;
    float baryy = 0;
    float baryx = 0;
    float baryz = 0;

	glm::vec3 modelpos;
	glm::mat4 model;
	int shading_mode = 1;
    std::vector<glm::vec3> pos;
    std::vector<GLuint> ind;
    Bumpy(std::string url);
	std::vector<glm::vec3> norms;
	std::vector<glm::vec3> phongnorms;

	std::vector<glm::vec3> posvec;
	std::vector<GLuint> indvec;

	glm::vec3 triangleNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{

		glm::vec3 u = a - b;
		glm::vec3 v = b - c;
		return glm::cross(u, v);
	}
	void calcNormal() {

		norms.clear();
		norms.resize(pos.size());
		phongnorms.resize(norms.size());
		std::vector<int> counters;
		counters.resize(pos.size());
		for (int i = 0; i < ind.size(); i += 3) {
			int i0 = ind[i];
			int i1 = ind[i + 1];
			int i2 = ind[i + 2];

			glm::vec3 pos0 = pos.at(i0);
			glm::vec3 pos1 = pos.at(i1);
			glm::vec3 pos2 = pos.at(i2);
			glm::vec3 N = triangleNormal(pos0, pos1, pos2);
			N = glm::normalize(N);
			phongnorms[i0] += N;
			phongnorms[i1] += N;
			phongnorms[i2] += N;
			norms[i0] = N;
			norms[i1] = N;
			norms[i2] = N;

			counters[i0]++;
			counters[i1]++;
			counters[i2]++;
		}
		//
		for (int i = 0; i < static_cast<int>(norms.size()); ++i) {
			//phongnorms[i] = norms[i];
			if (counters[i] > 0) {
				
				phongnorms[i] /=  counters[i];
			}
			else {
				glm::normalize(phongnorms[i]);

			}
				
		}
	}
    void pushVec();
};