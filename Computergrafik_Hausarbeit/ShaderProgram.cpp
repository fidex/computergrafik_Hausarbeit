//
//  ShaderProgram.cpp
//  Computergrafik06
//
//  Created by Thomas Stiene on 15.12.14.
//  Copyright (c) 2014 Thomas Stiene. All rights reserved.
//
#include <GL/glew.h>
#include "ShaderProgram.h"




ShaderProgram::ShaderProgram(){
    
}

ShaderProgram::~ShaderProgram(){
    
}

bool ShaderProgram::load(const char *VertexShader, const char *FragmentShader){
    
        
    this->m_VertexShader   = glCreateShader(GL_VERTEX_SHADER);    
    this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   
    
    std::cout << "loading shaders" << std::endl;
    loadVertexShader(VertexShader);
    loadFragmentShader(FragmentShader);
    std::cout << "compiling shaders" << std::endl;
    compile();
    
    glUseProgram(m_ShaderProgram);
    
    printProgramInfoLog(m_ShaderProgram);
    printShaderInfoLog(m_VertexShader);
    printShaderInfoLog(m_FragmentShader);
    
    return true;
}

bool ShaderProgram::loadVertexShader(const char *VertexShader){
    std::ifstream t(VertexShader);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    const char * vv = str.c_str();
    std::cout << "Loading Vertex Shader : "<< str.c_str() << std::endl;
    GLint length = str.length();
    glShaderSource(m_VertexShader, 1, &vv, &length);
    return true;
}

bool ShaderProgram::loadFragmentShader(const char *FragmentShader){
    std::ifstream t(FragmentShader);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    const char * ff = str.c_str();
    GLint length = str.length();
    glShaderSource(m_FragmentShader, 1, &ff, &length);
    return true;
}

bool ShaderProgram::compile(std::string* CompileErrors){
    m_ShaderProgram = glCreateProgram();
    
    glCompileShader(m_FragmentShader);
    glCompileShader(m_VertexShader);
    
    glAttachShader(m_ShaderProgram,m_VertexShader);
    glAttachShader(m_ShaderProgram,m_FragmentShader);
    
    glLinkProgram(m_ShaderProgram);
    std::cout<< "shaders compiled" <<std::endl;
    return true;
}

void ShaderProgram::activate() const{
    glUseProgram(m_ShaderProgram);
}

void ShaderProgram::deactivate() const{
    glUseProgram(0);
}

void ShaderProgram::printShaderInfoLog(GLuint obj)
{
    printf("ShaderInfo:\n");
    
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void ShaderProgram::printProgramInfoLog(GLuint obj)
{
    printf("ProgramInfo:\n");
    
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

GLint ShaderProgram::getParameterID(const char *ParameterName) const{
    return glGetUniformLocation(m_ShaderProgram, ParameterName);
}

void ShaderProgram::setParameter(GLint ID, int Param){
    glUniform1i(ID, Param);
}

//void ShaderProgram::setParameter(GLint ID, const Matrix &Param){
//    //glUniformMatrix4fv(ID, 1, GL_FALSE, Param);
//}
