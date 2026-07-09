#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include "mesh.h"



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
                vertex.faces.push_back(face[0]);
                vertex.faces.push_back(face[1]);
                vertex.faces.push_back(face[2]);
            }
            else if (face.size() == 4)
            {
            // Triangle 1
                vertex.faces.push_back(face[0]);
                vertex.faces.push_back(face[1]);
                vertex.faces.push_back(face[2]);

            // Triangle 2
                vertex.faces.push_back(face[0]);
                vertex.faces.push_back(face[2]);
                vertex.faces.push_back(face[3]);
            }
            
        }
    }

    return vertex;
}