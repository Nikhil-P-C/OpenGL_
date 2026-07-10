#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <fstream>

#include "tinyglTF.h"
#include "stb_image.h"
#include "Shader.h"
#include "mesh.h"

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static float yaw = -90.0f;
    static float pitch = 0.0f;
    static float lastX = 400.0f;
    static float lastY = 300.0f;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed because screen Y increases downward

    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}
unsigned int glError(){
    unsigned int error =glGetError();

    if(error){
        std::cout<<"OpenGl ERROR:"<<error<<"\n";
        return error;
    }
    return 0;
}
int main () {
    
    mesh OBJ;
    // Vertex vertex = OBJ.loadOBJModel("res/model/Male.OBJ");


    // stbi_set_flip_vertically_on_load(1);
    // int height =0, width =0,nrChannel=0;
    // int FORMAT =0;
    // unsigned char* data =stbi_load("res/3.jpg",&width,&height,&nrChannel,0);
    // std::cout<<nrChannel;
    // if(!data){
    //     std::cout<<"error image didnt load\n";
    // }
    // if(nrChannel == 3)FORMAT =GL_RGB;
    // if(nrChannel == 4)FORMAT =GL_RGBA;

    if (!glfwInit()) {
        std::cout<< "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1280, 720);
    glfwSwapInterval(0);
    if(glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }  
    if (!window) {
        glfwTerminate();
        return -1;
    }
    Shader shader("res/shader/shaders.shader");
    


    tinygltf::TinyGLTF loader;
    tinygltf::Model model3d;
    std::cout <<"file exist:" << std::filesystem::exists("res/model/2b_nier_automata.glb") << '\n';
    std::ifstream file("res/model/2b_nier_automata.glb", std::ios::binary);
    std::cout <<"file opened:" << file.is_open() << '\n';
    std::cout <<"is regular file:" << std::filesystem::is_regular_file("res/model/2b_nier_automata.glb") << '\n';
    std::string err, warn;
    std::cout<<std::filesystem::current_path();
    bool ok = loader.LoadBinaryFromFile(
        &model3d,
        &err,
        &warn,
        "res/model/leon.glb"
    );


    if (!warn.empty())
        std::cout << warn << '\n';

    if (!err.empty())
        std::cout << err << '\n';

    if (!ok)
        std::cout << "Failed to load model\n";
    std::cout << "Meshes: " << model3d.meshes.size() << '\n';

    for (size_t i = 0; i < model3d.meshes.size(); i++)
    {
        std::cout << "Mesh " << i
              << " primitives: "
              << model3d.meshes[i].primitives.size()
              << '\n';
    }
    const tinygltf::Mesh& mesh = model3d.meshes[0];

    const tinygltf::Primitive& primitive = mesh.primitives[0];

    const tinygltf::Accessor& positionAccessor =model3d.accessors[primitive.attributes.find("POSITION")->second];

    const tinygltf::BufferView& positionView =model3d.bufferViews[positionAccessor.bufferView];

    const tinygltf::Buffer& positionBuffer =model3d.buffers[positionView.buffer];

    const float* positions =reinterpret_cast<const float*>(&positionBuffer.data[positionView.byteOffset + positionAccessor.byteOffset]);
    
    const tinygltf::Accessor& indexAccessor =model3d.accessors[primitive.indices];

    const tinygltf::BufferView& indexView =model3d.bufferViews[indexAccessor.bufferView];

    const tinygltf::Buffer& indexBuffer =model3d.buffers[indexView.buffer];

    std::vector<unsigned int> indices;



    


    if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
    {
        const uint16_t* src =
        reinterpret_cast<const uint16_t*>(
            &indexBuffer.data[indexView.byteOffset +
                              indexAccessor.byteOffset]);

        for (size_t i = 0; i < indexAccessor.count; i++)indices.push_back(src[i]);
    }
    else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
    {
        const uint32_t* src =
        reinterpret_cast<const uint32_t*>(&indexBuffer.data[indexView.byteOffset +indexAccessor.byteOffset]);

    for (size_t i = 0; i < indexAccessor.count; i++)
        indices.push_back(src[i]);
    }


    const tinygltf::Material& material =model3d.materials[primitive.material];
    int imgComponent = 3;
	int imgHeight = 0;
	int imgWidth = 0;
	const char* imgData = nullptr;
    if (primitive.material >= 0)
    {
        const tinygltf::Material& material = model3d.materials[primitive.material];

        int textureIndex = material.pbrMetallicRoughness.baseColorTexture.index;

        if (textureIndex >= 0)
        {
            const tinygltf::Texture& texture = model3d.textures[textureIndex];
            const tinygltf::Image& image = model3d.images[texture.source];
            imgComponent = image.component;
            imgHeight = image.height;
            imgWidth = image.width;
            imgData = reinterpret_cast<const char*>(image.image.data());
        }
        else std::cout << "no texture image\n";
    }
    auto uvIt = primitive.attributes.find("TEXCOORD_0");
    const float* texCoords;
    int texCount = 0;
    if (uvIt != primitive.attributes.end())
    {
        const tinygltf::Accessor& texAccessor =model3d.accessors[uvIt->second];
        const tinygltf::BufferView& texView =model3d.bufferViews[texAccessor.bufferView];
        const tinygltf::Buffer& texBuffer =model3d.buffers[texView.buffer];
        texCoords = reinterpret_cast<const float*>(&texBuffer.data[texView.byteOffset + texAccessor.byteOffset]);

		texCount = texAccessor.count;
    }
    else
    {
        std::cout << "Model has no texture coordinates.\n";
    }
    


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
    unsigned int indices1[]={
        0,1,3,
        1,2,3
    };
    std::cout << "Index count: " << indices.size() << '\n';
    std::cout << "\nPosition ptr: " << (void*)positions << '\n';
    unsigned int VAO =0;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glError();
    std::cout << "count = " << positionAccessor.count << '\n';
    std::cout << "type = " << positionAccessor.type << '\n';
    std::cout << "componentType = " << positionAccessor.componentType << '\n';
    std::cout << "byteStride = " << positionView.byteStride << '\n';
    
    unsigned int VBO =0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,positionAccessor.count * 3 * sizeof(float),positions,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    unsigned int uvVBO=0 ;
    glGenBuffers(1, &uvVBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER,texCount * 2 * sizeof(float),texCoords,GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glError();
    unsigned int IBO =0;
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),indices.data(),GL_STATIC_DRAW);
    glError();

    unsigned int texture =0;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glError();

    GLenum format = imgComponent == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D,0,format,imgWidth,imgHeight,0,format,GL_UNSIGNED_BYTE,imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glError();
    

    
    
    std::mt19937 rng(12345);

    std::uniform_real_distribution<float> xDist(-20.0f, 20.0f);
    std::uniform_real_distribution<float> yDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> zDist(-20.0f, 20.0f);

    std::vector<glm::mat4> modelMatrices;

    for (int i = 0; i <20; i++)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f),
                                         glm::vec3(xDist(rng),yDist(rng),zDist(rng)));
        modelMatrices.push_back(model);
    }

    
    glm::mat4 projection;
    projection = glm::perspective(45.00f,800.0f/600.0f,0.1f,5000.0f);
      
    
    
    shader.use();
    shader.setInt(0,"u_texture");
    glError();

    float increment =1.0f;
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 10.0f);
    // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); already defined globally
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    double lasttime = glfwGetTime();
    double nowtime =0;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    while(!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);
        
        
        glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
        
        {  
        glError();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        glBindVertexArray(VAO);
        
        for(auto& model : modelMatrices){
            // model = glm::rotate(model,1.0f,glm::vec3(0.0f,1.0f,0.0f));
            glm::mat4 PVM = projection* view* model;
            int PVMloc=0;
            PVMloc= glGetUniformLocation(shader.m_ID,"u_PVM");
            glUniformMatrix4fv(PVMloc,1,GL_FALSE,glm::value_ptr(PVM));
            glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0); 
        }
        
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }
        float speed = 0.2f;
        float yaw = -90.0f;
        float pitch = 0.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += speed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= speed * cameraFront;

        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += speed * right;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= speed * right;

        nowtime = glfwGetTime();
        double deltaTime = nowtime - lasttime;
        lasttime =nowtime;
        std::cout<<"frametime:"<<deltaTime*1000<<"\n";

    }    
        // cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        // cameraFront.y = sin(glm::radians(pitch));
        // cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        // cameraFront = glm::normalize(cameraFront);
    glfwTerminate();
    return 0;
}