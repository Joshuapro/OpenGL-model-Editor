#include "Helpers.h"

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

void VertexArrayObject::init()
{
  glGenVertexArrays(1, &id);
  check_gl_error();
}

void VertexArrayObject::bind()
{
  glBindVertexArray(id);
  check_gl_error();
}

void VertexArrayObject::free()
{
  glDeleteVertexArrays(1, &id);
  check_gl_error();
}

void VertexBufferObject::init()
{
  glGenBuffers(1,&id);
  check_gl_error();
}

void VertexBufferObject::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER,id);
  check_gl_error();
}

void VertexBufferObject::free()
{
  glDeleteBuffers(1,&id);
  check_gl_error();
}

bool Program::init(
  const std::string &vertex_shader_string,
  const std::string &fragment_shader_string,
  const std::string &fragment_data_name)
{
  using namespace std;
  vertex_shader = create_shader_helper(GL_VERTEX_SHADER, vertex_shader_string);
  fragment_shader = create_shader_helper(GL_FRAGMENT_SHADER, fragment_shader_string);

  if (!vertex_shader || !fragment_shader)
    return false;

  program_shader = glCreateProgram();

  glAttachShader(program_shader, vertex_shader);
  glAttachShader(program_shader, fragment_shader);

  glBindFragDataLocation(program_shader, 0, fragment_data_name.c_str());
  glLinkProgram(program_shader);

  GLint status;
  glGetProgramiv(program_shader, GL_LINK_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];
    glGetProgramInfoLog(program_shader, 512, NULL, buffer);
    cerr << "Linker error: " << endl << buffer << endl;
    program_shader = 0;
    return false;
  }

  check_gl_error();
  return true;
}

void Program::bind()
{
  glUseProgram(program_shader);
  check_gl_error();
}

GLint Program::attrib(const std::string &name) const
{
  return glGetAttribLocation(program_shader, name.c_str());
}

GLint Program::uniform(const std::string &name) const
{
  return glGetUniformLocation(program_shader, name.c_str());
}

GLint Program::bindVertexAttribArray(
        const std::string &name, VertexBufferObject& VBO) const
{
  GLint id = attrib(name);
  if (id < 0)
    return id;
  if (VBO.id == 0)
  {
    glDisableVertexAttribArray(id);
    return id;
  }
  VBO.bind();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  check_gl_error();

  return id;
}

void Program::free()
{
  if (program_shader)
  {
    glDeleteProgram(program_shader);
    program_shader = 0;
  }
  if (vertex_shader)
  {
    glDeleteShader(vertex_shader);
    vertex_shader = 0;
  }
  if (fragment_shader)
  {
    glDeleteShader(fragment_shader);
    fragment_shader = 0;
  }
  check_gl_error();
}

GLuint Program::create_shader_helper(GLint type, const std::string &shader_string)
{
  using namespace std;
  if (shader_string.empty())
    return (GLuint) 0;

  GLuint id = glCreateShader(type);
  const char *shader_string_const = shader_string.c_str();
  glShaderSource(id, 1, &shader_string_const, NULL);
  glCompileShader(id);

  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE)
  {
    char buffer[512];
    if (type == GL_VERTEX_SHADER)
      cerr << "Vertex shader:" << endl;
    else if (type == GL_FRAGMENT_SHADER)
      cerr << "Fragment shader:" << endl;
    else if (type == GL_GEOMETRY_SHADER)
      cerr << "Geometry shader:" << endl;
    cerr << shader_string << endl << endl;
    glGetShaderInfoLog(id, 512, NULL, buffer);
    cerr << "Error: " << endl << buffer << endl;
    return (GLuint) 0;
  }
  check_gl_error();

  return id;
}

void _check_gl_error(const char *file, int line)
{
  GLenum err (glGetError());

  while(err!=GL_NO_ERROR)
  {
    std::string error;

    switch(err)
    {
      case GL_INVALID_OPERATION:      error         = "INVALID_OPERATION";      break;
      case GL_INVALID_ENUM:           error         = "INVALID_ENUM";           break;
      case GL_INVALID_VALUE:          error         = "INVALID_VALUE";          break;
      case GL_OUT_OF_MEMORY:          error         = "OUT_OF_MEMORY";          break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
    }

    std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
    err = glGetError();
  }
}


Bunny::Bunny(std::string url){
	objsize = 1;
	uid = 0;
	angleY = 0;
  angleX = 0;
	model = glm::mat4(1.0f);
	modelpos = glm::vec3(0.0f);
  std::ifstream imageFile;
  imageFile.open(url, std::ios::in | std::ios::binary);
  if (imageFile.is_open()){
    string str;
    getline(imageFile,str);
    getline(imageFile,str);
    string cur = "";
    string first;
    string second;
    int count = 0;
    for(int i = 0; i < str.size(); i++){
      if (str[i] == ' '){
        if (count == 0){
          count += 1;
          first = cur;
          cur = "";
        }else{
          second = cur;
          break;
        }
      }else{
        cur = cur + str[i];
      }
    }
    float minix = 9999;
    float maxix = -9999;
    float miniy = 9999;
    float maxiy = -9999;
    float miniz = 9999;
    float maxiz = -9999;


    int counter = 0;

    while(getline(imageFile,str)){

      if (counter < stoi(first)){
        float f;
        float second;
        float third;
        int count = 0;
        string num = "";
        for(int i = 0; i < str.size(); i++){
          if (str[i] == ' '){
            if (count == 0){
              f = stof(num) * 1.5;
              baryx += f;
              minix = min(minix,f);
              maxix = max(maxix,f);
              num = "";
              count += 1;
            }
            else if(count == 1){
              second = stof(num) * 1.5;
              baryy += second;
              miniy = min(miniy,second);
              maxiy = max(maxiy,second);
              num = "";
              count += 1;
            }
          }else{
            num += str[i];
          }
        }
        third = stof(num) * 1.5;
        miniz = min(miniz,third);
        maxiz = max(maxiz,third);
        baryz += third;
        pos.push_back(glm::vec3(f,second,third));
      }else{
        string num = "";
        bool start = true;
        for (int i = 0; i < str.size(); i++){
          if (str[i] == ' '){
            if (start){
              start = false;
              num = "";
            }else{
              ind.push_back((GLuint)stoi(num));
              num = "";
            }
          }else{
            num += str[i];
          }
        }
        ind.push_back((GLuint)stoi(num));
      }
      counter += 1;
    }
    baryy = (maxiy + miniy)/2;
    baryx = (maxix + minix)/2;
    baryz = (maxiz + miniz)/2;
    // cout << miniz << endl;
    cout << (maxiy + miniy)/2 << endl;

    for(int i = 0; i < pos.size(); i++){
      pos[i][0] = pos[i][0] - baryx;
      pos[i][1] = pos[i][1] - baryy;
      pos[i][2] = pos[i][2] - baryz;
    }

  }
  

}


void Bunny::pushVec(){
      posstart = posvec.size();
      for (int i = 0; i < pos.size() ; i+=3){
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
		  posvec.push_back(pos[i+1]);
		  if (shading_mode == 0) {
			  posvec.push_back(norms[i + 1]);
		  }
		  else {
			  posvec.push_back(phongnorms[i + 1]);
		  }
		  posvec.push_back(pos[i+2]);

		  if (shading_mode == 0) {
			  posvec.push_back(norms[i + 2]);
		  }
		  else {
			  posvec.push_back(phongnorms[i + 2]);
		  }
      }

      indstart = indvec.size();
      for (int i = 0; i < ind.size(); i++){
          indcount += 1;
          indvec.push_back(ind[i] + posstart);
      }
  }



void Bumpy::pushVec(){
      posstart = posvec.size();
	  for (int i = 0; i < pos.size() ; i += 3) {
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
		  posvec.push_back(pos[i + 2]);

		  if (shading_mode == 0) {
			  posvec.push_back(norms[i + 2]);
		  }
		  else {
			  posvec.push_back(phongnorms[i + 2]);
		  }
	  }

      indstart = indvec.size();
      for (int i = 0; i < ind.size(); i++){
          indcount += 1;
          indvec.push_back(ind[i] + posstart);
      }
  }

Bumpy::Bumpy(std::string url){
	objsize = 1;
	angleY = 0;
  angleX = 0;
	uid = 0;
	model = glm::mat4(1.0f);
	modelpos = glm::vec3(0.0f);
  std::ifstream imageFile;
  imageFile.open(url, std::ios::in | std::ios::binary);
  if (imageFile.is_open()){
    string str;
    getline(imageFile,str);
    getline(imageFile,str);
    string cur = "";
    string first;
    string second;
    int count = 0;
    float minix = 9999;
    float maxix = -9999;
    float miniy = 9999;
    float maxiy = -9999;
    float miniz = 9999;
    float maxiz = -9999;
    for(int i = 0; i < str.size(); i++){
      if (str[i] == ' '){
        if (count == 0){
          count += 1;
          first = cur;
          cur = "";
        }else{
          second = cur;
          break;
        }
      }else{
        cur = cur + str[i];
      }
    }


    int counter = 0;

    while(getline(imageFile,str)){
      if (counter < stoi(first)){
        float f;
        float second;
        float third;
        int count = 0;
        string num = "";
        for(int i = 0; i < str.size(); i++){
          if (str[i] == ' '){
            if (count == 0){
              f = stof(num) * 0.03;
              baryx += f;
              minix = min(minix,f);
              maxix = max(maxix,f);
              num = "";
              count += 1;
            }
            else if(count == 1){
              second = stof(num) * 0.03;
              baryy += second;
              miniy = min(miniy,second);
              maxiy = max(maxiy,second);
              num = "";
              count += 1;
            }
          }else{
            num += str[i];
          }
        }
        third = stof(num) * 0.03;
        miniz = min(miniz,third);
        maxiz = max(maxiz,third);
        baryz += third;
        pos.push_back(glm::vec3(f,second,third));
      }else{
        string num = "";
        bool start = true;
        for (int i = 0; i < str.size(); i++){
          if (str[i] == ' '){
            if (start){
              start = false;
              num = "";
            }else{
              ind.push_back((GLuint)stoi(num));
              num = "";
            }
          }else{
            num += str[i];
          }
        }
        ind.push_back((GLuint)stoi(num));
      }
      counter += 1;
    }
    
    // cout << minix << endl;

    baryy = baryy/stoi(first);
    baryx = baryx/stoi(first);
    baryz = baryz/stoi(first);

    for(int i = 0; i < pos.size(); i++){
      pos[i][0] = pos[i][0] - baryx;
      pos[i][1] = pos[i][1] - baryy;
      pos[i][2] = pos[i][2] - baryz;
    }

  }
  

}


