#include <vector>
#include <string>
#include <glm/glm.hpp>

struct Vertex
{
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec2> texCoord;
    std::vector<unsigned int> faces;
};
class mesh{
public:
    Vertex loadOBJModel(const std::string& filepath);
    

private:
unsigned int m_VBO , m_VAO , m_IBO, m_textureID;
Vertex m_model;
};