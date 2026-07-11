#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

class Model{
public:
    Model(char* path){
        loadModel(path);
    }
    void draw(Shader shader){
        for(unsigned int i = 0; i< m_meshes.size();i++){
            m_meshes[i].draw(shader);
        }
    }
private:
void loadModel(std::string path);
void processNode(aiNode *node,const aiScene *scene);
void processMesh(aiMesh *mesh,const aiScene *scene);

std::vector<Texture> loadTextureMaterial(aiMaterial *material, aiTextureType *type, std::string typeName);


private:
    std::vector<mesh> m_meshes;
    std::string m_directory;
};