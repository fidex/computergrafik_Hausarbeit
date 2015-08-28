//
//  ShaderProgram.h
//  Computergrafik06
//
//  Created by Thomas Stiene on 15.12.14.
//  Copyright (c) 2014 Thomas Stiene. All rights reserved.
//

#ifndef __Computergrafik06__ShaderProgram__
#define __Computergrafik06__ShaderProgram__


#include <stdio.h>
#include <string>
#include <GL/glut.h>
#include "Vector.h"
#include "Color.h"
#include "Matrix.h" 
#include <fstream>

class ShaderProgram{
public:
    ShaderProgram();
    ~ShaderProgram();
    bool load(const char* VertexShader, const char* FragmentShader);
    bool loadVertexShader(const char* VertexShader);
    bool loadFragmentShader(const char* FragmentShader);
    bool compile(std::string* CompileErrors=NULL);
    
    GLint getParameterID(const char* ParameterName) const;
    
    void setParameter(GLint ID, float Param);
    void setParameter(GLint ID, int Param);
    void setParameter(GLint ID, const Vector& Param);
    void setParameter(GLint ID, const Color& Param);
    void setParameter(GLint ID, const Matrix& Param);
    
    void activate() const;
    void deactivate() const;
    
private:
    GLint m_VertexShader;
    GLint m_FragmentShader;
    GLint m_ShaderProgram;
    
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    
};

#endif /* defined(__Computergrafik06__ShaderProgram__) */
