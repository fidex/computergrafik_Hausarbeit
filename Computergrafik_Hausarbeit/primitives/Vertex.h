/* 
 * File:   Vertex.h
 * Author: phil
 *
 * Created on 7. September 2015, 21:44
 */

#ifndef VERTEX_H
#define	VERTEX_H
#include "../Vector.h"
#include <iostream>

struct Vertex
{
    Vertex(){
    
    }
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT)
    {
        Position = p;
        Normal = n;
        TexcoordS = TexS;
        TexcoordT = TexT;
    }
    
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
    bool hasNormal;
    bool hasTexcoords;
    bool operator==(const Vertex &v) const{
        if(this->Normal == v.Normal)
        if(this->Position == v.Position)
        if(this->TexcoordS == v.TexcoordS)
        if(this->TexcoordT == v.TexcoordT)
            return true;

        return false;
    }
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


#endif	/* VERTEX_H */

