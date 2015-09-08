/* 
 * File:   Ground.h
 * Author: phil
 *
 * Created on 7. September 2015, 21:09
 */

#ifndef GROUND_H
#define	GROUND_H

#include "GL/glew.h"
#include "primitives/Vertex.h"
#include "ShaderProgram.h"
#include <unordered_map>
#include <vector>


class Ground {
public:
    Ground();
    Ground(GLuint XSize, GLuint ZSize, GLuint maxHeight);
    ~Ground();
    void loadHeightmap(const char* filename);
    void loadShaders(const char* vertexShader, const char* fragmentShader);
    void draw();
    
    GLuint getMaxHeight() const;
    void setMaxHeight(GLuint maxHeight);

    GLuint getXSize() const {
        return m_XSize;
    }

    void setXSize(GLuint XSize) {
        m_XSize = XSize;
    }

    GLuint getZSize() const {
        return m_ZSize;
    }

    void setZSize(GLuint ZSize) {
        m_ZSize = ZSize;
    }


private:
    void loadBMP(const char* filename);
    void buffer();
    GLfloat interpolateRGB(GLfloat inVal, GLfloat min, GLfloat max);
    GLfloat interpolate(GLfloat inVal, GLfloat minInRange, GLfloat maxInRange, GLfloat minOutRange, GLfloat maxOutRange);
    
    GLuint m_MaxHeight;
    GLuint m_XSize;
    GLuint m_ZSize;
    
    Vertex* m_Vertices = NULL;
    std::vector<GLuint> m_Indices;
    GLuint m_VertexCount = 0;
    
    GLuint m_Width = 0;
    GLuint m_Height = 0;
    
    GLuint m_VBID;
    GLuint m_IBID;
    
    ShaderProgram m_Shader;
    
    bool isBuffered = false;
    bool shaderLoaded = false;
};

#endif	/* GROUND_H */

