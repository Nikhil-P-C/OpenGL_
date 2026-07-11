#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <Shader.h>
struct Vertex
{
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> texCoord;
    
};
struct Texture{
    unsigned int ID;
    std::string type;
};
class mesh{
public:
    Vertex loadOBJModel(const std::string& filepath);
    void draw(Shader shader);
    mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures);
private:
    void setupMesh();
private:
    unsigned int m_VBO , m_VAO , m_IBO, m_textureID;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_faces;
    std::vector<Texture> m_textures;
};