#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

enum ShaderType{
    NONE =-1,
    VERTEX =0,
    FRAGMENT =1
};
Shader::Shader(const std::string& path){
    std::cout<<path<<"\n";
    ShaderType type =NONE;
    std::ifstream shadersource(path);
    std::string line;
    std::cout << std::filesystem::current_path() << '\n';

    std::stringstream ss[2];
    while(std::getline(shadersource,line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex")!=std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
            continue;
        }
        if(type != ShaderType::NONE){
            ss[type]<<line<<"\n";
        }
    }
    std::string vertexShaderString = ss[ShaderType::VERTEX].str();
    std::string fragmentShaderString = ss[ShaderType::FRAGMENT].str();
	std::cout << "vertex:\n" << vertexShaderString << "\n";
	std::cout << "fragment:\n" << fragmentShaderString << "\n";
    const char* vertexShaderSource = vertexShaderString.c_str();
    const char* fragmentShaderSource =fragmentShaderString.c_str();

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(m_vertexShader);

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(m_fragmentShader);

    int success;
    char log[512];
    glGetShaderiv(m_fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(m_fragmentShader,512,NULL,log);
        std::cout<<"ERROR:"<<log<<"\n";
    }
    
    glGetShaderiv(m_vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(m_vertexShader,512,NULL,log);
        std::cout<<"ERROR:"<<log<<"\n";
    }
    m_ID =glCreateProgram();
    glAttachShader(m_ID,m_vertexShader);
    glAttachShader(m_ID,m_fragmentShader);
    glLinkProgram(m_ID);
}
void Shader::use(){
    glUseProgram(m_ID);

}
void Shader::setFloat(float value,const std::string& name){
    glUniform1f(glGetUniformLocation(m_ID,name.c_str()),value);
}
void Shader::setInt(int value,const std::string& name){
    glUniform1i(glGetUniformLocation(m_ID,name.c_str()),value);
}