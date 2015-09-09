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
#include <sstream>


class Ground {
public:
    Ground();
    Ground(GLuint XSize, GLuint ZSize, GLuint maxHeight);
    ~Ground();
    void loadHeightmap(const char* filename);
    void loadTexture(const char* filename);
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
    GLfloat getDM1TillingU() const {
        return m_DM1TillingU;
    }

    void setDM1TillingU(GLfloat DM1TillingU) {
        m_DM1TillingU = DM1TillingU;
    }

    GLfloat getDM1TillingV() const {
        return m_DM1TillingV;
    }

    void setDM1TillingV(GLfloat DM1TillingV) {
        m_DM1TillingV = DM1TillingV;
    }

    
    void setDefaultMaterial();
    
    std::string toString() const;

private:
    void loadBMP(const char* filename);
    void buffer();
    GLfloat interpolateRGB(GLfloat inVal, GLfloat min, GLfloat max);
    GLfloat interpolate(GLfloat inVal, GLfloat minInRange, GLfloat maxInRange, GLfloat minOutRange, GLfloat maxOutRange);
    
    GLuint m_MaxHeight;
    GLuint m_XSize;
    GLuint m_ZSize;
    std::string m_HMFilename, m_MMFilename, m_DM1Filename, m_DM2Filename;
    GLfloat m_DM1TillingU, m_DM1TillingV, m_DM2TillingU, m_DM2TillingV;
    
    Material m_Material;
    
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
    bool textureLoaded = false;
};

#endif	/* GROUND_H */

