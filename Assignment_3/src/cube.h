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

	glm::vec3 modelpos;
	glm::vec3 color;
	float angleY;
	float angleX;
	float objsize;
	int uid;
	glm::mat4 model;
	int shading_mode = 1;
	int display_mode = 0;
	std::vector<glm::vec3> posvec;
	std::vector<GLuint> indvec;
	std::vector<glm::vec3> norms;
	std::vector<glm::vec3> phongnorms;
    // Cube(int start, int end){

    // }
	glm::vec3 triangleNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	{

		glm::vec3 u = a - b;
		glm::vec3 v = b - c;
		return glm::cross(u, v);
	}
	void calcNormal() {

		norms.clear();
		norms.resize(pos.size());
		std::vector<int> counters;
		counters.resize(pos.size());
		phongnorms.resize(norms.size());
		for (int i = 0; i < ind.size(); i += 3) {
			int i0 = ind[i];
			int i1 = ind[i + 1];
			int i2 = ind[i + 2];

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
    Cube(){
		angleY = 0;
		angleX = 0;
		objsize = 0.1;
		model = glm::mat4(1.0f);
		modelpos = glm::vec3(0.0f);
		color = glm::vec3(0.73f);


		calcNormal();
		posstart = posvec.size();
		for (int i = 0; i < pos.size(); i += 3) {
			if (i + 1 > pos.size()) {
				break;
			}
			poscount += 3;
			posvec.push_back(pos[i]);
			if (shading_mode == 0) {
				posvec.push_back(norms[i]);
			}
			else {
				posvec.push_back(phongnorms[i]);
			}
			if (i + 1 >= pos.size()) break;
			posvec.push_back(pos[i + 1]);
			if (shading_mode == 0) {
				posvec.push_back(norms[i + 1]);
			}
			else {
				posvec.push_back(phongnorms[i + 1]);
			}
			if (i + 2 >= pos.size()) break;
			posvec.push_back(pos[i + 2]);

			if (shading_mode == 0) {
				posvec.push_back(norms[i + 2]);
			}
			else {
				posvec.push_back(phongnorms[i + 2]);
			}
		}

		indstart = indvec.size();
		for (int i = 0; i < ind.size(); i++) {
			indcount += 1;
			indvec.push_back(ind[i] + posstart);
		}
    }

    
    std::vector<glm::vec3> pos = {
		glm::vec3(-1,-1,-1),
		glm::vec3(-1,-1, 1),
		glm::vec3(1,-1,-1),
		glm::vec3(1,-1, 1),
		glm::vec3(-1, 1,-1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1,-1),
		glm::vec3(1, 1, 1),
    };

    std::vector<GLuint> ind = {
		// bottom
			0,2,3,
			3,1,0,
			// top
			4,5,7,
			7,6,4,
			// left
			0,1,5,
			5,4,0,
			// right
			2,6,7,
			7,3,2,
			// back
			0,4,6,
			6,2,0,
			// front
			1,3,7,
			7,5,1,
    };

    

};

#endif
