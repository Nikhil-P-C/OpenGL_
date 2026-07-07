#pragma once
#include <string>
class Shader{
public:
    Shader(const std::string& path);
    ~Shader() =default;
    unsigned int getShaderProgram(){
        return m_ID;
    }
    void use();

    void setInt(int value,const std::string& uniform);
    void setFloat(float value,const std::string& uniform);
    void setBool(bool value,const std::string& uniform);
    void setUnsignedInt(unsigned int value,const std::string& uniform);

    void setMat4f();
    
public:
    unsigned int m_ID =0;
private:
    
    unsigned int m_fragmentShader =0;
    unsigned int m_vertexShader =0;
};