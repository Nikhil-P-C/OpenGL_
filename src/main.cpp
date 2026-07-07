#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"

unsigned int glError(){
    unsigned int error =glGetError();

    if(error){
        std::cout<<"OpenGl ERROR:"<<error<<"\n";
        return error;
    }
    return 0;
}
int main () {
    
    
    stbi_set_flip_vertically_on_load(1);
    int height =0, width =0,nrChannel=0;
    int FORMAT =0;
    unsigned char* data =stbi_load("res/3.jpg",&width,&height,&nrChannel,0);
    std::cout<<nrChannel;
    if(!data){
        std::cout<<"error image didnt load\n";
    }
    if(nrChannel == 3)FORMAT =GL_RGB;
    if(nrChannel == 4)FORMAT =GL_RGBA;

    if (!glfwInit()) {
        std::cout<< "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 600);
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
    
    glEnable(GL_DEPTH_TEST);
    float vertices[] = {
        //veritces              //tex coord
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3 *sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int IBO =0;
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    unsigned int texture =0;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,FORMAT,width,height,0,FORMAT,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    
    
    glm::mat4 model =glm::mat4(1.0f);
    model = glm::rotate(model,80.0f,glm::vec3(1.0f,0.0f,0.0f));

    glm::mat4 view =glm::mat4(1.0f);
    view =glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
    

    glm::mat4 projection;
    projection = glm::perspective(45.00f,800.0f/600.0f,0.1f,100.0f);
    for (int i = 0; i < 4; ++i){
        for (int c = 0; c < 4; ++c)
        {
            for (int r = 0; r < 4; ++r)
            std::cout << projection[c][r] << ' ';
            std::cout << '\n';
        }
    }   
    std::cout << GLM_VERSION_MAJOR << "."
          << GLM_VERSION_MINOR << "."
          << GLM_VERSION_PATCH << '\n';
    std::cout << glm::radians(45.0f) << '\n';
    float aspect = 800.0f / 600.0f;
    std::cout << aspect << '\n';
    shader.use();
        shader.setInt(0,"u_texture");
        
    float increment =1.0f;
        

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model = glm::rotate(model,increment,glm::vec3(1.0f,1.0f,1.0f));
        int viewloc= glGetUniformLocation(shader.m_ID,"u_view");
        glUniformMatrix4fv(viewloc,1,GL_FALSE,glm::value_ptr(view));
        
        int modelloc=0;
        modelloc = glGetUniformLocation(shader.m_ID,"u_model");
        glUniformMatrix4fv(modelloc,1,GL_FALSE,glm::value_ptr(model));

        int projloc =glGetUniformLocation(shader.m_ID,"u_projection");
        glUniformMatrix4fv(projloc,1,GL_FALSE,glm::value_ptr(projection));
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        
        glError();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        
        

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);

        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

    }
    glfwTerminate();
    return 0;
}