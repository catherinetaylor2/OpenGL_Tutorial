#include <glew.h>
#include <glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdio>
#include <vector>
#include "shader.hpp"
#include <glm/gtx/transform.hpp>

int main(){

    if(!glfwInit()){ // initialize GLFW
        std::cout<<"Error: failed to initialize GLFW \n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    int width =1080, height = 720;
    window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);
    if(window==NULL){
        std::cout<<"Error: failed to open window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if(glewInit()!=GLEW_OK){
        std::cout<<"Error: failed to initialize GLEW \n";
        return -1;
    }
    //MVP MATRICES:
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians (45.0f),         //FOV
        (float)width/(float)height, // Aspect Ratio. 
        0.1f,        // Near clipping plane. 
        100.0f       // Far clipping plane.
    );
    glm::mat4 ViewMatrix =  glm::lookAt(
        glm::vec3(4,3,3), // position of camera
        glm::vec3(0,0,0),  // look at vector
        glm::vec3(0,1,0)  //look up vector
    );
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::mat4 MVP = projectionMatrix*ViewMatrix*ModelMatrix;

   

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f,0.0f,
    };
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    do{
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,  
            (void*)0
        );
        glDrawArrays(GL_TRIANGLES, 0,3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && glfwWindowShouldClose(window)==0);

    return 0;
}