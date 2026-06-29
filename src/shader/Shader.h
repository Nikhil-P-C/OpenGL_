#include <tuple>
#include <string>
class Shader{
public:
    Shader(const std::string& path);
    ~Shader();

    void use();

    void setInt(int value,const std::string&);
    void setFloat(float value,const std::string&);
    void setBool(bool value,const std::string&);
    void setUnsignedInt(unsigned int value,const std::string&);
    

private:
    unsigned int m_fragmentShader =0;
    unsigned int m_vertexShader =0;
};