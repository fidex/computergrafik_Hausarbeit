//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <GL/glew.h>
#include <iostream>
#include "Vector.h"
#include "Color.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Material.h"
#include "ShaderProgram.h"
#include "primitives/Vertex.h"


struct MaterialGroup{
    GLuint offset, count;
    std::string name;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    Vector Min;
    Vector Max;
};

class Model
{
public:
    Model();
    ~Model();
//    Model(const Model& orig);
//    Model & operator=(Model other);

    const BoundingBox& boundingBox() const;
    bool loadOBJ( const char* Filename, bool FitSize=true);
    bool loadShaders(const char* VertexShader, const char* FragmentShader);
//    bool loadVertexShader(const char* VertexShader);
//    bool loadFragmentShader(const char* FragmentShader);
    void draw();
    void draw(Vector translation, Vector scaling, Vector rotation, GLfloat angle);
    void buffer();
    void drawLines() const;
    void drawTriangles() const;
protected:
    void createCube();
    void parseFaceVertex(std::string &line, GLuint *v, GLuint *vt, GLuint *vn);
    void loadMTL(const char* filename);
    bool setMaterial(std::string matName);
    std::unordered_map<std::string, Material> m_pMaterials;
    unsigned int m_MaterialCount;
    Vertex* m_pVertices;
    std::vector<GLuint> m_pIndices;
    std::vector<MaterialGroup> m_pMGroups;
    unsigned int m_VertexCount;
    unsigned int m_IndexCount;
    GLuint m_vertexBufferID = 0;
    GLuint m_indexBufferID = 0;
    ShaderProgram m_shader;
    BoundingBox m_Box;
    bool isBuffered=false;
    bool shaderLoaded=false;
};

#endif /* defined(__RealtimeRending__Model__) */
