//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <iostream>
#include "Vector.h"
#include "Color.h"
#include <string>
#include <map>
#include <vector>
#include "Material.h"

struct Vertex
{
    Vertex();
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT);
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
    bool hasNormal;
    bool hasTexcoords;
    bool operator==(const Vertex &v) const;
};

struct VertexHasher
{
  std::size_t operator()(const Vertex& v) const
  {
    using std::size_t;
    using std::hash;
    

    return ((hash<float>()(v.Position.X))
             ^ (hash<float>()(v.Position.Y))
             ^ (hash<float>()(v.Position.Z)));
  }
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
    const BoundingBox& boundingBox() const;
    bool loadOBJ( const char* Filename, bool FitSize=true);
    void draw() const;
    void buffer();
    void drawLines() const;
    void drawTriangles() const;
protected:
    void createCube();
    void parseFaceVertex(std::string &line, GLuint *v, GLuint *vt, GLuint *vn);
    void loadMTL(const char* filename);
    std::vector<Material> m_pMaterials;
    unsigned int m_MaterialCount;
    Vertex* m_pVertices;
    std::vector<GLuint> m_pIndices;
    unsigned int m_VertexCount;
    unsigned int m_IndexCount;
    GLuint m_vertexBufferID = 0;
    GLuint m_indexBufferID = 0;
    BoundingBox m_Box;
};

#endif /* defined(__RealtimeRending__Model__) */
