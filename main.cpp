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
#include "BITMAP.hpp"
#include <glm/gtx/transform.hpp>

glm::vec3 position = glm::vec3(0,0,5);
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float FoV = 45.0f;
float speed= 3.0f;
float mouseSpeed = 0.0005f;
float lastTime =glfwGetTime();

glm::mat4 projectionMatrix = glm::perspective(
    glm::radians (45.0f),         //FOV
    (float)1080/(float)720, // Aspect Ratio. 
    0.1f,        // Near clipping plane. 
    100.0f       // Far clipping plane.
);
glm::mat4 ViewMatrix =  glm::lookAt(
    glm::vec3(4,3,3), // position of camera
    glm::vec3(0,0,0),  // look at vector
    glm::vec3(0,1,0)  //look up vector
);

void computeMatricesFromInputs(GLFWwindow *window){
    double xpos, ypos;
    glfwGetCursorPos(window,&xpos, &ypos);
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    horizontalAngle += mouseSpeed*deltaTime  * float(1024/2 - xpos );
    verticalAngle   += mouseSpeed*deltaTime  * float( 768/2 - ypos );
    lastTime=currentTime;
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );
    glm::vec3 up = glm::cross( right, direction );
    projectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    ViewMatrix       = glm::lookAt(
        position,           // Camera is here
        position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
};
glm::mat4 getProjectionMatrix(){
    return projectionMatrix;
}
glm::mat4 getViewMatrix(){
    return ViewMatrix;
}

int main(){

    unsigned char * texture_data;
	int texture_width, texture_height;
	texture_data = readBMP("texture.bmp", &texture_width, &texture_height);

    float *V, *N, *VT;
    int F, *FV, *FN, *FT;
    ObjFile mesh("cube.obj");
	mesh.get_mesh_data(mesh, &FV, &FN, &FT, &VT, &N, &V, &F);
	std::cout<<"tree built \n";
     int number_of_faces = mesh.get_number_of_faces();
    int number_of_vertices = mesh.get_number_of_vertices();

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
   
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

  static const GLfloat g_vertex_buffer_data[] = { //cube
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*number_of_vertices*sizeof(float),  &V[0], GL_DYNAMIC_DRAW);

   static const GLfloat g_uv_buffer_data[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    do{
        computeMatricesFromInputs(window);
        glm::mat4 projectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix =  getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glm::mat4 MVP = projectionMatrix*ViewMatrix*ModelMatrix;

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

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
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,  
            (void*)0
        );
        glDrawArrays(GL_TRIANGLES, 0,3*12);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    while(glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS && glfwWindowShouldClose(window)==0);

    delete [] texture_data;
    ObjFile::clean_up(V,N, VT, FV, FN, FT);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);

    return 0;
}