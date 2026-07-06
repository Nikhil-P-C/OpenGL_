#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
int main () {
    stbi_set_flip_vertically_on_load(1);
    int height , width ,nrChannel; 
    unsigned char* data =stbi_load("res/4.png",&width,&height,&nrChannel,0);
    std::cout<<nrChannel;
    if(!data){
        std::cout<<"error image didnt load\n";
    }

    if (!glfwInit()) {
        std::cout<< "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Example", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    if(glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }  
    if (!window) {
        glfwTerminate();
        return -1;
    }
    Shader shader("res/shader/shaders.shader");
    unsigned int texture =0;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);


    float vertices[] = {
        //veritces   //tex coord
         0.5f, 0.5f, 1.0f, 1.0f,
         0.5f,-0.5f, 1.0f, 0.0f,
        -0.5f,-0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,
    };
    unsigned int indices[]={
        0,1,3,
        1,2,3
    };

    unsigned int VAO =0;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    
    
    unsigned int VBO =0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2 *sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int IBO =0;
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    
    float greenColor =0.0f;
    float increment =0.01f;
    


    while(!glfwWindowShouldClose(window)) {
        shader.use();
        shader.setInt(0,"u_texture");
        shader.setFloat(greenColor,"u_color");
        greenColor+= increment;
        if(greenColor>=1.0f|| greenColor<0.0f)increment =( increment * -1);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
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