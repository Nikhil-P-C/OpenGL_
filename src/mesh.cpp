#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include "mesh.h"
#include "glError.h"
mesh::mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures){
    m_vertices = vertices;
    m_faces = indices;
    m_textures =textures;

    setupMesh();
}
void mesh::setupMesh(){
    glCall(glGenVertexArrays(1,&m_VAO));
    glCall(glGenBuffers(1,&m_VBO));
    glCall(glGenBuffers(1,&m_IBO));

    glCall(glBindVertexArray(m_VAO));

    glCall(glBindBuffer(GL_ARRAY_BUFFER,m_VBO));
    glCall(glBufferData(GL_ARRAY_BUFFER,m_vertices.size()*sizeof(Vertex),&m_vertices[0],GL_STATIC_DRAW));

    glCall(glBindBuffer(GL_ARRAY_BUFFER,m_IBO));
    glCall(glBufferData(GL_ARRAY_BUFFER,m_faces.size() * sizeof(unsigned int), &m_faces[0],GL_STATIC_DRAW));

    glCall(glEnableVertexAttribArray(0));
    glCall(glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) 0));

    glCall(glEnableVertexAttribArray(1));
    glCall(glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,normal)));

    glCall(glEnableVertexAttribArray(2));
    glCall(glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,texCoord)));

    glCall(glBindVertexArray(0));
}
void mesh::draw(){
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
Vertex mesh::loadOBJModel(const std::string& filepath){
    Vertex vertex;
    std::ifstream model = std::ifstream(filepath);
    std::string line;
    while(std::getline(model,line)){
        if (line.rfind("v ", 0) == 0)
        {   
            std::istringstream iss(line);
            std::string type;
            float x,y,z;
            iss>>type>>x>>y>>z;
            vertex.position.push_back(glm::vec3(x,y,z));
        }
        else if (line.rfind("vt ", 0) == 0)
        {

        }
        else if (line.rfind("vn ", 0) == 0)
        {

        }
        if (line.rfind("f ", 0) == 0)
        {   
            std::istringstream iss(line);
            std::string type;
            iss>> type; // Skip "f"
            std::vector<float> face;
            std::string vec;
            while (iss >> vec)
            {
                size_t slash = vec.find('/');

                unsigned int index;

                if (slash != std::string::npos)
                    index = std::stoi(vec.substr(0, slash));
                else
                     index = std::stoi(vec); // Handles "f 1 2 3"

                index--; // OBJ is 1-based
                face.push_back(index);
            }
            if (face.size() == 3)
            {
                m_faces.push_back(face[0]);
                m_faces.push_back(face[1]);
                m_faces.push_back(face[2]);
            }
            else if (face.size() == 4)
            {
            // Triangle 1
                m_faces.push_back(face[0]);
                m_faces.push_back(face[1]);
                m_faces.push_back(face[2]);

            // Triangle 2
                m_faces.push_back(face[0]);
                m_faces.push_back(face[2]);
                m_faces.push_back(face[3]);
            }
            
        }
    }

    return vertex;
}