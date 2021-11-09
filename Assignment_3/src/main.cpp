// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"
#include "ebo.h"
#include "cube.h"


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#else
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif

// OpenGL Mathematics Library
#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <fstream>
#include <sstream>
#include <iostream>
// Timer
#include <chrono>
#include <iostream>
#include <exception>



int shading_mode = 2;
int globalPickedId = -1;
std::vector<Cube> cubes;
std::vector<Bunny> bunnies;
std::vector<Bumpy> bumpies;

int total_object = 150;
// VertexBufferObject wrapper
VertexBufferObject VBO;
EBO ebo;
// Contains the vertex positions
//Eigen::MatrixXf V(2,3);
const unsigned int width = 640;
const unsigned int height = 480;

std::vector<glm::vec3> V;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);
glm::vec3 objectColor(0.73f);
glm::vec3 target(0.0f, 0.0f,0.0f);
glm::vec3 init = glm::vec3(0.0f,0.0f,3.0f);
glm::mat4 projection;
bool ortho = false;




float hori = 0;
float verti = 0;
float r = 1;
int display_mode = 0;
Program program;
Program programPick;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

 void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
 {
     // Get the position of the mouse in the window
     double xpos, ypos;
     glfwGetCursorPos(window, &xpos, &ypos);
     // Get the size of the window
     int width, height;
     glfwGetWindowSize(window, &width, &height);
     // Convert screen position to world coordinates
     double xworld = ((xpos/double(width))*2)-1;
     double yworld = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw

	
	int prev = globalPickedId;
	

	glReadPixels(xpos, height-1-ypos,1,1,GL_STENCIL_INDEX,GL_INT, &globalPickedId);
	

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		if (globalPickedId != prev && globalPickedId != 0){
			// std::cout << globalPickedId << std::endl;
			for (int i = 0; i < cubes.size(); i++) {
				if (cubes[i].uid == globalPickedId) {
					cubes[i].color = glm::vec3(0, 1, 0);
				}
				if (cubes[i].uid == prev) {
					cubes[i].color = glm::vec3(0.73f);
				}
			}
			for (int i = 0; i < bunnies.size(); i++) {
				if (bunnies[i].uid == globalPickedId) {
					bunnies[i].color = glm::vec3(0, 1, 0);
				}
				if (bunnies[i].uid == prev) {
					bunnies[i].color = glm::vec3(0.73f);
				}
			}
			for (int i = 0; i < bumpies.size(); i++) {
				if (bumpies[i].uid == globalPickedId) {
					bumpies[i].color = glm::vec3(0, 1, 0);
				}
				if (bumpies[i].uid == prev) {
					bumpies[i].color = glm::vec3(0.73f);
				}
			}
		}else if (globalPickedId == 0){
			for (int i = 0; i < cubes.size(); i++) {
				cubes[i].color = glm::vec3(0.73f);
			}
			for (int i = 0; i < bunnies.size(); i++) {
				bunnies[i].color = glm::vec3(0.73f);
			}
			for (int i = 0; i < bumpies.size(); i++) {
				bumpies[i].color = glm::vec3(0.73f);
			}
		}
	}

	
	


 }

 void scaleObject(int dir) {
	 float at = 0;
	 if (dir == 0) {//up
		 at += 0.1;

	 }
	 else if (dir == 1) {//up
		 at -= 0.1;

	 }
	 if (globalPickedId < 0) return;
	 for (int i = 0; i < cubes.size(); i++) {
		 if (cubes[i].uid == globalPickedId) {
			 cubes[i].objsize += at;
			 return;
		 }
	 }
	 for (int i = 0; i < bunnies.size(); i++) {
		 if (bunnies[i].uid == globalPickedId) {
			 bunnies[i].objsize += at;
			 return;
		 }
	 }
	 for (int i = 0; i < bumpies.size(); i++) {
		 if (bumpies[i].uid == globalPickedId) {
			 bumpies[i].objsize += at;
		 }
	 }



 }
void rotateObject(int dir) {
	float at = 0;
	float dy = 0;
	if (dir == 0) {//up
		at += 0.1;
	}
	if (dir == 1) {//up
		at -= 0.1;
	}
	if (dir == 2){
		dy += 0.1;
	}

	if (dir == 3){
		dy -= 0.1;
	}
	if (globalPickedId < 0) return;
	for (int i = 0; i < cubes.size(); i++) {
		if (cubes[i].uid == globalPickedId) {
			cubes[i].angleY += at;
			cubes[i].angleX += dy;
			return;
		}
	}
	for (int i = 0; i < bunnies.size(); i++) {
		if (bunnies[i].uid == globalPickedId) {
			bunnies[i].angleY += at;
			bunnies[i].angleX += dy;
			return;
		}
	}
	for (int i = 0; i < bumpies.size(); i++) {
		if (bumpies[i].uid == globalPickedId) {
			bumpies[i].angleY += at;
			bumpies[i].angleX += dy;
		}
	} 
	
	

}
void deleteObject() {
	if (globalPickedId < 0) return;
	for (int i = 0; i < cubes.size(); i++) {
		if (cubes[i].uid == globalPickedId) {
			cubes[i].uid = -1;
			return;
		}
	}
	for (int i = 0; i < bunnies.size(); i++) {
		if (bunnies[i].uid == globalPickedId) {
			bunnies[i].uid = -1;
			return;
		}
	}
	for (int i = 0; i < bumpies.size(); i++) {
		if (bumpies[i].uid == globalPickedId) {
			bumpies[i].uid = -1;
		}
	}
}

void moveObject(int dir) {
	if (globalPickedId < 0) return;
	for (int i = 0; i < cubes.size(); i++) {
		if (cubes[i].uid == globalPickedId) {
			if (dir == 0) {//up
				cubes[i].modelpos.y += 0.1;
			}
			else if (dir == 1) {//up
				cubes[i].modelpos.y -= 0.1;
			}
			else if (dir == 2) {//up
				cubes[i].modelpos.x -= 0.1;
			}
			else if (dir == 3) {//up
				cubes[i].modelpos.x += 0.1;
			}
			return;
		}
	}
	for (int i = 0; i < bunnies.size(); i++) {
		if(bunnies[i].uid == globalPickedId){
			if (dir == 0) {
				bunnies[i].modelpos.y += 0.1;
			}
			else if (dir == 1) {
				bunnies[i].modelpos.y -= 0.1;
			}
			else if (dir == 2) {
				bunnies[i].modelpos.x -= 0.1;
			}
			else if (dir == 3) {
				bunnies[i].modelpos.x += 0.1;
			}
			return;
		}
	}
	for (int i = 0; i < bumpies.size(); i++) {
		if (bumpies[i].uid == globalPickedId) {
			if (dir == 0) {
				bumpies[i].modelpos.y += 0.1;
			}
			else if (dir == 1) {
				bumpies[i].modelpos.y -= 0.1;
			}
			else if (dir == 2) {
				bumpies[i].modelpos.x -= 0.1;
			}
			else if (dir == 3) {
				bumpies[i].modelpos.x += 0.1;
			}
		}
	}
	
	

}

void change(int num){
	if (globalPickedId < 0) return;
	for (int i = 0; i < cubes.size(); i++) {
		if (cubes[i].uid == globalPickedId) {
			cubes[i].uid = -1;
			Cube hi;
			hi.uid = total_object++;
			hi.shading_mode = num;
			hi.modelpos = cubes[i].modelpos;
			hi.color = glm::vec3(0, 1, 0);
			hi.objsize = cubes[i].objsize;
			globalPickedId = hi.uid;
			hi.angleY = cubes[i].angleY;
			hi.angleX = cubes[i].angleX;
			cubes.push_back(hi);
			return;
		}
	}
	for (int i = 0; i < bunnies.size(); i++) {
		if (bunnies[i].uid == globalPickedId) {
			bunnies[i].uid = -1;
			Bunny bun("../data/bunny.off");
			bun.shading_mode = num;
			bun.uid = total_object++;
			bun.modelpos = bunnies[i].modelpos;
			bun.color = glm::vec3(0, 1, 0);
			bun.objsize = bunnies[i].objsize;
			bun.angleY = bunnies[i].angleY;
			bun.angleX = bunnies[i].angleX;
			globalPickedId = bun.uid;
			bun.calcNormal();
			bun.pushVec();
			bunnies.push_back(bun);
			return;
		}
	}
	for (int i = 0; i < bumpies.size(); i++) {
		if (bumpies[i].uid == globalPickedId) {
			bumpies[i].uid = -1;
			Bumpy bum("../data/bumpy_cube.off");
			bum.color = glm::vec3(0, 1, 0);
			bum.shading_mode = num;
			bum.uid = total_object++;
			globalPickedId = bum.uid;
			bum.modelpos = bumpies[i].modelpos;
			bum.objsize = bumpies[i].objsize;
			bum.angleY = bumpies[i].angleY;
			bum.angleX = bumpies[i].angleX;
			bum.calcNormal();
			bum.pushVec();
			bumpies.push_back(bum);
			return;
		}
	}
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Update the position of the first vertex if the keys 1,2, or 3 are pressed
    switch (key)
    {
        case GLFW_KEY_1:
            if (action == GLFW_PRESS){
                Cube hi;
				hi.uid = total_object++;
				hi.shading_mode = shading_mode;
                cubes.push_back(hi);
            }
            break;
        case GLFW_KEY_2:
            if (action == GLFW_PRESS){
                Bunny bun("../data/bunny.off");
				bun.shading_mode = shading_mode;
				bun.uid = total_object++;
				bun.calcNormal();
				bun.pushVec();
                bunnies.push_back(bun);
            }
            break;
        case GLFW_KEY_3:
            if (action == GLFW_PRESS){
                Bumpy bum("../data/bumpy_cube.off");
				bum.shading_mode = shading_mode;
				bum.uid = total_object++;
				bum.calcNormal();
				bum.pushVec();
                bumpies.push_back(bum);
            }
			break;
        case GLFW_KEY_T:
            if (action == GLFW_PRESS){
				deleteObject();
            }
			break;
		case GLFW_KEY_P:
			if (action == GLFW_PRESS) {
				change(2);
			}
			break;
		case GLFW_KEY_F:
			
			if (action == GLFW_PRESS) {
				change(1);
			}
			break;
		case GLFW_KEY_N:
			if (action == GLFW_PRESS) {
				change(0);
			}
			break;
		case GLFW_KEY_W:
			moveObject(0);
			break;
		case GLFW_KEY_S:
			moveObject(1);
			break;
		case GLFW_KEY_A:
			moveObject(2);
			break;
		case GLFW_KEY_D:
			moveObject(3);
			break;
		case GLFW_KEY_E:
			rotateObject(0);
			break;
		case GLFW_KEY_R:
			rotateObject(1);
			break;
		case GLFW_KEY_I:
			rotateObject(2);
			break;
		case GLFW_KEY_O:
			rotateObject(3);
			break;
		case GLFW_KEY_G:
			scaleObject(0);
			break;
		case GLFW_KEY_H:
			scaleObject(1);
			break;
		case GLFW_KEY_UP:
			if (action == GLFW_PRESS){
				ortho = !ortho;
			}
			break;
		case GLFW_KEY_Z:
			init.x += 0.1;
			break;
		case GLFW_KEY_X:
			init.x -= 0.1;
			break;
		case GLFW_KEY_C:
			init.y += 0.1;
			break;
		case GLFW_KEY_V:
			init.y -= 0.1;
			break;
		case GLFW_KEY_4:
			objectColor = glm::vec3(1,0,0);
			break;
		case GLFW_KEY_5:
			objectColor = glm::vec3(1, 1, 0);
			break;
		case GLFW_KEY_6:
			objectColor = glm::vec3(1, 0,1);
			break;
        default:
            break;
    }

    // Upload the change to the GPU
    VBO.update(V);
}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Ensure that we get at least a 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
	// glfwGetPrimarymonitor()
    window = glfwCreateWindow(width, height, "Hello World", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      }
      glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();

   
    ebo.init();
    ebo.bind();

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
    VBO.init();

    V.resize(1);
    VBO.update(V);

    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
	
	const GLchar* pickvertex_shader =
		"#version 150 core\n"
		"in vec3 position; \n"
		"in vec3 normal;\n"
		"out vec3 Normal;\n"
		"uniform mat4 MVP; \n"
		"void main() {\n"
		"	Normal = normal;\ngl_Position = MVP * vec4(position, 1); \n"
		"}\n";

	const GLchar* pickfragment_shader =
		"#version 150 core\n"
		"out vec4 outColor; \n"
	"uniform vec3 PickingColor; \n"
	"void main() {\n"
	"	outColor = vec4(PickingColor,1); \n"
	"}\n";
	programPick.init(pickvertex_shader, pickfragment_shader, "outColor");
	check_gl_error();
	programPick.bind();
	check_gl_error();

	// The vertex shader wants the position of the vertices as an input.
	// The following line connects the VBO we defined above with the position "slot"
	// in the vertex shader
	//programPick.bindVertexAttribArray("normal", VBO);
	programPick.bindVertexAttribArray("position", VBO);
	

		const GLchar* vertex_shader =
		"#version 150 core\n"
		"in vec3 position;\n"
		"in vec3 normal;\n"

		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"out vec3 flatColor;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 objectColor;\n"

		"void main()\n"
		"{\n"
		"    FragPos = vec3(model * vec4(position, 1.0));\n"
		"    Normal = mat3(transpose(inverse(model))) * normal;\n"
		"    // ambient\n"
		"    float ambientStrength = 0.1;\n"
		"    vec3 ambient = ambientStrength * lightColor;\n"

		"    // diffuse \n"
		"    vec3 norm = normalize(Normal);\n"
		"    vec3 lightDir = normalize(lightPos - FragPos);\n"
		"    float diff = max(dot(norm, lightDir), 0.0);\n"
		"    vec3 diffuse = diff * lightColor;\n"

		"    // specular\n"
		"    float specularStrength = 0.5;\n"
		"    vec3 viewDir = normalize(viewPos - FragPos);\n"
		"    vec3 reflectDir = reflect(-lightDir, norm);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
		"    vec3 specular = specularStrength * spec * lightColor;\n"

		"    flatColor = (ambient + diffuse + specular) * objectColor;\n"

		"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
		"}\n";
    const GLchar* fragment_shader =
		" #version 150 core\n"
		"out vec4 outColor;\n"

		"in vec3 Normal;\n"
		"in vec3 FragPos;\n"
		"in vec3 flatColor;\n"

		"uniform vec3 lightPos;\n"
		"uniform vec3 viewPos;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 objectColor;\n"
		"uniform int shading_mode;\n"
		"uniform int display_mode;\n"

		"void main()\n"
		"{"
		"    // ambient\n"
		"    float ambientStrength = 0.1;\n"
		"    vec3 ambient = ambientStrength * lightColor;\n"

		"    // diffuse \n"
		"    vec3 norm = normalize(Normal);\n"
		"    vec3 lightDir = normalize(lightPos - FragPos);\n"
		"    float diff = max(dot(norm, lightDir), 0.0);\n"
		"    vec3 diffuse = diff * lightColor;\n"

		"    // specular\n"
		"    float specularStrength = 0.5;\n"
		"    vec3 viewDir = normalize(viewPos - FragPos);\n"
		"    vec3 reflectDir = reflect(-lightDir, norm);\n"
		"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
		"    vec3 specular = specularStrength * spec * lightColor;\n"

		"    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
		"	if(shading_mode == 0){\n"
		"		outColor = vec4(flatColor, 1.0);\n"
		"	}else{\n"
		"		outColor = vec4(result, 1.0);\n"
		"	}\n"
		"if(display_mode==1){outColor = vec4(1,0,0, 1.0);}\n"

		"}\n";
	 
    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader, fragment_shader,"outColor");
    program.bind();

    // The vertex shader wants the position of the vertices as an input.
    // The following line connects the VBO we defined above with the position "slot"
    // in the vertex shader
    program.bindVertexAttribArray("position",VBO);

    // Save the current time --- it will be used to dynamically change the triangle color
    auto t_start = std::chrono::high_resolution_clock::now();

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glViewport(0, 0, width, height);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

    // Camera cam(width,height,glm::vec3(0.0f,0.0f,3.0f));

	

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        VAO.bind();
        ebo.bind();
		// if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {		
		// 	globalPickedId = 1000;
		// }
        // Bind your program
        program.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
        // Set the uniform value depending on the time difference
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        // glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

    

        // Clear the framebuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 proj = glm::mat4(1.0f);


		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		static float n = 0;
		n += 0.5;

		glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
		glm::vec3 viewPos(0.0f, -1.0f, 0.0f);
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);



		view = glm::lookAt(init,glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));
		if (ortho){
			projection = glm::ortho(-2.0f, 2.0f, -1.5f ,1.5f ,0.1f, 5.0f);
		}
		else{
			projection = glm::perspective(glm::radians(45.0f), (float) width / height, 0.1f, 100.0f); 
		}



	



		glUniformMatrix4fv(program.uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
 
		// glUniform3fv(program.uniform("objectColor"), 1, glm::value_ptr(objectColor));
		glUniform3fv(program.uniform("lightColor"), 1, glm::value_ptr(lightColor));
		glUniform3fv(program.uniform("viewPos"), 1, glm::value_ptr(viewPos));
		glUniform3fv(program.uniform("lightPos"),1, glm::value_ptr(lightPos));
		glUniform1i(program.uniform("shading_mode"), shading_mode);

        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		
		
       // glUniformMatrix4fv(program.uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
		check_gl_error();
      
       




       

		// if (display_mode == 0) {
		// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// }
		// else {
		// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// }

		glUniform1i(program.uniform("display_mode"), 0);
        

        
        for (int i = 0; i < cubes.size(); i++){
			if (cubes[i].uid == -1) {
				continue;
			}
			if (cubes[i].shading_mode == 0){
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}else{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glUniform3fv(program.uniform("objectColor"), 1, glm::value_ptr(cubes[i].color));
			glStencilFunc(GL_ALWAYS, cubes[i].uid, -1);	
			VBO.update(cubes[i].posvec);
			ebo.update(cubes[i].indvec);
			auto model = glm::translate(glm::mat4(1.0f), cubes[i].modelpos);
			model = glm::rotate(model, cubes[i].angleY, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cubes[i].angleX, glm::vec3(1.0f, 0.0f, 0.0f));
			float objsize = cubes[i].objsize;
			model = glm::scale(model, glm::vec3(objsize, objsize, objsize));
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, cubes[i].indcount, GL_UNSIGNED_INT, (GLvoid*)(cubes[i].indstart*sizeof(GL_UNSIGNED_INT)));
        }


        for (int i = 0; i < bunnies.size();i++){      
			if (bunnies[i].uid == -1) {
				continue;
			}

			if (bunnies[i].shading_mode == 0){
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}else{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			glUniform3fv(program.uniform("objectColor"), 1, glm::value_ptr(bunnies[i].color));
			glStencilFunc(GL_ALWAYS, bunnies[i].uid, -1);	
			

			VBO.update(bunnies[i].posvec);
			ebo.update(bunnies[i].indvec);
			auto model = glm::translate(glm::mat4(1.0f), bunnies[i].modelpos);
			model = glm::rotate(model, bunnies[i].angleY, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, bunnies[i].angleX, glm::vec3(1.0f, 0.0f, 0.0f));
			float objsize = bunnies[i].objsize;
			model = glm::scale(model, glm::vec3(objsize, objsize, objsize));
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			//VBO.updateN(bunnies[i].norms);
			//glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(bunnies[0].model));
            glDrawElements(GL_TRIANGLES, bunnies[i].indcount, GL_UNSIGNED_INT, (GLvoid*)((bunnies[i].indstart)*sizeof(GL_UNSIGNED_INT)));
        }

        for (int i = 0; i < bumpies.size();i++){     
			if (bumpies[i].uid == -1) {
				continue;
			}
			if (bumpies[i].shading_mode == 0){
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}else{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			glUniform3fv(program.uniform("objectColor"), 1, glm::value_ptr(bumpies[i].color));
			glStencilFunc(GL_ALWAYS, bumpies[i].uid, -1);
		
			VBO.update(bumpies[i].posvec);
			ebo.update(bumpies[i].indvec);
			auto model = glm::translate(glm::mat4(1.0f), bumpies[i].modelpos);
			model = glm::rotate(model, bumpies[i].angleY, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, bumpies[i].angleX, glm::vec3(1.0f, 0.0f, 0.0f));
			float objsize = bumpies[i].objsize;
			model = glm::scale(model, glm::vec3(objsize, objsize, objsize));

			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, bumpies[i].indcount, GL_UNSIGNED_INT, (GLvoid*)((bumpies[i].indstart)*sizeof(GL_UNSIGNED_INT)));
        }
		
		glUniform1i(program.uniform("display_mode"), 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (int i = 0; i < cubes.size(); i++) {
			if (cubes[i].uid == -1) {
				continue;
			}
			if (cubes[i].shading_mode != 1){
				continue;
			}
			VBO.update(cubes[i].posvec);
			ebo.update(cubes[i].indvec);
			auto model = glm::translate(glm::mat4(1.0f), cubes[i].modelpos);
			model = glm::rotate(model, cubes[i].angleY, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, cubes[i].angleX, glm::vec3(1.0f, 0.0f, 0.0f));
			float objsize = cubes[i].objsize;
			model = glm::scale(model, glm::vec3(objsize, objsize, objsize));
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, cubes[i].indcount, GL_UNSIGNED_INT, (GLvoid*)(cubes[i].indstart * sizeof(GL_UNSIGNED_INT)));
		}


		for (int i = 0; i < bunnies.size(); i++) {
			if (bunnies[i].uid == -1) {
				continue;
			}
			if (bunnies[i].shading_mode != 1){
				continue;
			}

			VBO.update(bunnies[i].posvec);
			ebo.update(bunnies[i].indvec);
			auto model = glm::translate(glm::mat4(1.0f), bunnies[i].modelpos);
			model = glm::rotate(model, bunnies[i].angleY, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, bunnies[i].angleX, glm::vec3(1.0f, 0.0f, 0.0f));
			float objsize = bunnies[i].objsize;
			model = glm::scale(model, glm::vec3(objsize, objsize, objsize));
			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			//VBO.updateN(bunnies[i].norms);
			//glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(bunnies[0].model));
			glDrawElements(GL_TRIANGLES, bunnies[i].indcount, GL_UNSIGNED_INT, (GLvoid*)((bunnies[i].indstart) * sizeof(GL_UNSIGNED_INT)));
		}

		for (int i = 0; i < bumpies.size(); i++) {
			if (bumpies[i].uid == -1) {
				continue;
			}
			if (bumpies[i].shading_mode != 1){
				continue;
			}

			VBO.update(bumpies[i].posvec);
			ebo.update(bumpies[i].indvec);
			auto model = glm::translate(glm::mat4(1.0f), bumpies[i].modelpos);
			model = glm::rotate(model, bumpies[i].angleY, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, bumpies[i].angleX, glm::vec3(1.0f, 0.0f, 0.0f));
			float objsize = bumpies[i].objsize;
			model = glm::scale(model, glm::vec3(objsize, objsize, objsize));

			glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, bumpies[i].indcount, GL_UNSIGNED_INT, (GLvoid*)((bumpies[i].indstart) * sizeof(GL_UNSIGNED_INT)));
		}
	


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Deallocate opengl memory
    program.free();
    VAO.free();
    VBO.free();
    ebo.free();

    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}

