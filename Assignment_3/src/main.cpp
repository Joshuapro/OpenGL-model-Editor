// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"
#include "ebo.h"
#include "cube.h"
// #include "Bunny.h"


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
// Timer
#include <chrono>
#include <iostream>
#include <exception>

std::vector<glm::vec3> vertextPosition;
std::vector<GLuint> indices;
std::vector<Cube> cubes;
std::vector<Bunny> Bunnies;

// VertexBufferObject wrapper
VertexBufferObject VBO;

// Contains the vertex positions
//Eigen::MatrixXf V(2,3);
const unsigned int width = 640;
const unsigned int height = 480;

std::vector<glm::vec3> V;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 proj = glm::mat4(1.0f);
float hori = 0;
float verti = 0;
float r = 1;




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
// {
//     // Get the position of the mouse in the window
//     double xpos, ypos;
//     glfwGetCursorPos(window, &xpos, &ypos);
//     // Get the size of the window
//     int width, height;
//     glfwGetWindowSize(window, &width, &height);
//     // Convert screen position to world coordinates
//     double xworld = ((xpos/double(width))*2)-1;
//     double yworld = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw
//     // Update the position of the first vertex if the left button is pressed
//     if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//         V[0] = glm::vec2(xworld, yworld);
//     // Upload the change to the GPU
//     VBO.update(V);
// }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Update the position of the first vertex if the keys 1,2, or 3 are pressed
    switch (key)
    {
        case GLFW_KEY_1:
            if (action == GLFW_PRESS){
                Cube hi;
                cubes.push_back(hi);
            }
            break;
        case GLFW_KEY_2:
            if (action == GLFW_PRESS){
                Bunny bun("/Users/joshuayoung/Desktop/base3/Assignment_3/data/bunny.off");
                Bunnies.push_back(bun);
                std::cout << Bunnies.size() << std::endl;
            }
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
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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

    EBO ebo;
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
    Program program;
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec3 position;"
                    "uniform mat4 view;"
                    "uniform mat4 model;"
                    "uniform mat4 proj;"
                    "void main()"
                    "{"
                    "    gl_Position = proj * view * model * vec4(position, 1.0);"
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "out vec4 outColor;" 
                    "uniform vec3 triangleColor;"
                    "void main()"
                    "{"
                    "    outColor = vec4(triangleColor, 1.0);"
                    "}";

    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader,fragment_shader,"outColor");
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
    // glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        VAO.bind();
        ebo.bind();

        // Bind your program
        program.bind();

        // Set the uniform value depending on the time difference
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        // glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

        // Clear the framebuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);
        
        // glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);


        // view = glm::scale(glm::mat4(1.f), glm::vec3(r, r, 1.f));
        view = glm::translate(view,glm::vec3(verti,hori,0));
        // proj = glm::perspective(glm::radians(45.0f),(float)(width/height), 0.1f, 100.0f);
        model = glm::rotate(model,glm::radians(10.5f),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(17.5f),glm::vec3(0.0f,1.0f,0.0f));

        glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(program.uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(program.uniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));

        // glUniform1d(uniID,0.5);
        // view = glm::translate(view,glm::vec3(0.0f,0.5f,-2.0f));
        // proj = glm::perspective(glm::radians(45.0f), (float)(640/680),0.1f, 100);






        //cubes
        vertextPosition = {};
        indices = {};
        for(int i = 0; i < cubes.size(); i++){
            for(int j = 0; j < cubes[i].pos.size(); j++){
                vertextPosition.push_back(cubes[i].pos[j]);
            }
            for(int j = 0; j < cubes[i].ind.size(); j++){
                indices.push_back(cubes[i].ind[j]);
            }
        }



        //bunny
       



 
        VBO.update(vertextPosition);
        ebo.update(indices);

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);



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





        //  vertextPosition = {
        //     glm::vec3(-0.5,0.5,-0.5),
        //     glm::vec3(-0.5,0.5,0.5),
        //     glm::vec3(0.5,0.5,-0.5),
        //     glm::vec3(0.5,0.5,0.5),
        //     glm::vec3(-0.5,-0.5,-0.5),
        //     glm::vec3(-0.5,-0.5,0.5),
        //     glm::vec3(0.5,-0.5,-0.5),
        //     glm::vec3(0.5,-0.5,0.5),
        // };

        // indices = {
        //     0,1,2,
        //     1,2,3,
 
        //     4,5,6,
        //     5,6,7,
     
        //     0,1,5,
        //     0,4,5,
        
        //     2,3,7,
        //     2,6,7,
      
        //     0,2,6,
        //     0,4,6,
           
        //     1,5,7,
        //     1,3,7
        // };
