#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader/Shader.h"
int main () {
    if (!glfwInit()) {
        std::cout<< "Failed to initialize GLFW" << std::endl;
        return -1;
    }
  
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }  
    if (!window) {
        glfwTerminate();
        return -1;
    }
   const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location =0) in vec2 aPos;\n"
    "void main (){\n"
    "   gl_Position = vec4(aPos.x,aPos.y, 0.0, 1.0);\n"
    "}\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    
    const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 fcolor;\n"
    "void main (){\n"
    "   fcolor = vec4(1.0,0.5, 0.0, 1.0);\n"
    "}\0";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    int success;
    char log[512];
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,log);
        std::cout<<"ERROR:"<<log<<"\n";
    }
    
    unsigned int shaderProgram =glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    float vertices[] = {
        -0.5f,-0.5f,
        -0.5f, 0.5f,
         0.5f,-0.5f,
         0.5f,0.5f,
    };
    unsigned int indices[]={
        0,1,2,
        2,3,1
    };

    unsigned int VAO =0;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    
    
    unsigned int VBO =0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    unsigned int IBO =0;
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    

    glUseProgram(shaderProgram);
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    glfwTerminate();
    return 0;
}