//
//  Model.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include <time.h>
#include <vector>
#include <fstream>
#include <assert.h>
#include <math.h>
#include <map>
#include <float.h>
#include <cstdio>
#include <cstring>
#include <sstream>

#define toDigit(c) (c-'0')
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


Vertex::Vertex()
{
    
}

Vertex::Vertex( const Vector& p, const Vector& n, float TexS, float TexT)
{
    Position = p;
    Normal = n;
    TexcoordS = TexS;
    TexcoordT = TexT;
}

bool Vertex::operator==(const Vertex &v) const{
    if(this->Normal == v.Normal)
    if(this->Position == v.Position)
    if(this->TexcoordS == v.TexcoordS)
    if(this->TexcoordT == v.TexcoordT)
        return true;
    
    return false;
}


BoundingBox::BoundingBox()
{
}
BoundingBox::BoundingBox( const Vector& min, const Vector& max) : Min(min), Max(max)
{
}

Model::Model() : m_MaterialCount(0), m_VertexCount(0)
{

}

Model::~Model()
{
    if( m_pVertices)
        delete [] m_pVertices;
}

bool Model::loadOBJ( const char* Filename, bool FitSize)
{
    clock_t t = clock();
    
    std::ifstream file(Filename);
    std::string line;
    
    struct UVCoord{
        float s,t;
    };
    
    struct Face{
        GLuint vindex[3], uvindex[3], nindex[3];
        std::string matname;
    };
    
    std::vector<Vector> vertices;
    std::vector<UVCoord> uvs;
    std::vector<Vector> normals;
    std::vector<Face> faces;
    
    std::string currentMat = "";
    
    
    char type[7] = "aa";
    int hits = 0;
    
    int linecounter = 1;
    
    while(std::getline(file,line)){  //read line 
//        std::cout << "line: " << linecounter << std::endl;
        linecounter++;
        hits = sscanf(line.c_str(), "%6s ", type);
        //std::cout << type << std::endl;
        if(hits == 1){
        if(!strcmp(type, "v")){
            //std::cout << "Vertex!" << std::endl;
            float x;
            float y;
            float z;
            sscanf(line.c_str(), "%*s %f %f %f",&x,&y,&z);
            Vector v(x, y, z);
            vertices.push_back(v);
        }
        else if(!strcmp(type, "vt")){
            //std::cout << "UV!" << std::endl;
            float s;
            float t;

            sscanf(line.c_str(), "%*s %f %f",&s,&t);
            UVCoord uv;
            uv.s = s;
            uv.t = t;
            uvs.push_back(uv);
        }
        else if(!strcmp(type, "vn")){
            //std::cout << "Normal!" << std::endl;
            float x;
            float y;
            float z;

            sscanf(line.c_str(), "%*s %f %f %f",&x,&y,&z);
            Vector v(x, y, z);
            normals.push_back(v);
        }
        else if(!strcmp(type, "mtllib")){
            char filename[255];
            sscanf(line.c_str(), "%*s %s", filename);
            std::string objFilename = std::string(Filename);
            size_t path_end = objFilename.find_last_of("/\\");
            if(path_end != objFilename.npos){
                std::string path = objFilename.substr(0, path_end);
                std::cout << filename << std::endl;
                loadMTL((path+"/"+filename).c_str());
            }else{
                loadMTL(filename);
            }
        }
        else if(!strcmp(type, "usemtl")){
            char matname[255];
            sscanf(line.c_str(), "%*s %s", matname);
            currentMat = matname;
//            std::cout << "Using mat: " << currentMat << std::endl;
        }
        else if(!strcmp(type, "f")){
            //std::cout << "Face!" << std::endl;

            //f 1/2/1 2//5 3
            std::stringstream ssline(line);
            std::vector<std::string> values;
            std::string substr;
            getline( ssline, substr, ' ' );         //Remove Prefix
            while(ssline.good()){                   //Split Vertices
                getline( ssline, substr, ' ' );
                values.push_back( substr );
            }
            //{1/2/1 2//5 3}


            for(int i=1; i<(values.size()-1); i++){     //Convert xs to Triangles
                Face f;
                parseFaceVertex(values[0], &f.vindex[0], &f.uvindex[0], &f.nindex[0]);
                parseFaceVertex(values[i], &f.vindex[1], &f.uvindex[1], &f.nindex[1]);
                parseFaceVertex(values[i+1], &f.vindex[2], &f.uvindex[2], &f.nindex[2]);
                f.matname = currentMat;
                faces.push_back(f);
            }

        }
        }
    } //File processed
    
    //Indexing
    std::unordered_map<Vertex, int, VertexHasher> vertex_map;
    
    int vindex;
    int nindex;
    int uvindex;
    
    int index;
    int vertexcounter = 0;
    
    if(faces.size() > 0){
    MaterialGroup tmp_mat;
    tmp_mat.name = faces[0].matname;
    tmp_mat.offset = 0;
    
    for(int i=0; i<faces.size(); i++){
        if(tmp_mat.name.compare(faces[i].matname) != 0){        //Neue Materialgruppe
            //Anzahl Elemente der alten Gruppe setzen und speichern
            tmp_mat.count = m_pIndices.size();
            m_pMGroups.push_back(tmp_mat);
            
            //Name und Offset der neuen Gruppe setzen
            tmp_mat.name = faces[i].matname;
            tmp_mat.offset = m_pIndices.size();
        }
        
        for(int j=0; j < 3;j++){
            Vertex v;
            
            vindex = faces[i].vindex[j]-1;
            nindex = faces[i].nindex[j]-1;
            uvindex = faces[i].uvindex[j]-1;
            
            //Vertex parsen
            v.Position = vertices[vindex];
            if(!nindex < 0){
                v.Normal = normals[nindex];
                v.hasNormal = true;
            }else{
                v.hasNormal = false;
            }
            if(!uvindex < 0){
                v.TexcoordS = uvs[uvindex].s;
                v.TexcoordT = uvs[uvindex].t;
                v.hasTexcoords = true;
            }else{
                v.hasTexcoords = false;
            }
            
            std::pair<Vertex, int> data(v, vertexcounter);
            std::pair<std::unordered_map<Vertex, int, VertexHasher>::iterator, bool> indexData = vertex_map.insert(std::make_pair(v,vertexcounter));
            if(indexData.second){
 //               std::cout << "New Vertex inserted at " << vertexcounter << std::endl;
                index = vertexcounter;
                vertexcounter++;
            }else{
                index = indexData.first->second;
//                std::cout << "Vertex found at " << index << std::endl;
            }
            m_pIndices.push_back(index);
        }
        
    }
    //Anzahl der letzten Gruppe setzen und speichern
    tmp_mat.count = m_pIndices.size();
    m_pMGroups.push_back(tmp_mat);
    
//    m_pVertices.resize(vertex_map.size());
    m_VertexCount = vertex_map.size();
    m_pVertices = new Vertex[vertex_map.size()];
    for(auto itr:vertex_map){
        m_pVertices[itr.second] = itr.first;
    }
    }
    std::cout << "Took " << (double) (clock()-t)/CLOCKS_PER_SEC << "s to load " << Filename << std::endl;
    return true;
}

void Model::parseFaceVertex(std::string &values, GLuint *v, GLuint *vt, GLuint *vn){  
    std::stringstream ss(values);
    
    std::string s_v;
    std::string s_vt;
    std::string s_vn;

    getline(ss, s_v, '/');
    std::istringstream ( s_v ) >> *v;

    getline(ss, s_vt, '/');
    if(s_vt.compare("") != 0){
        std::istringstream ( s_vt ) >> *vt;
    }
    else{
        *vt = 0;
    }

    getline(ss, s_vn, '/');
    if(s_vn.compare("") != 0){
        std::istringstream ( s_vn ) >> *vn;
    }
    else{
        *vn = 0;
    }
//    std::cout << "Face Vertex:" << std::endl;
//    std::cout << "Pos: " << *v << std::endl;
//    std::cout << "Tex: " << *vt << std::endl;
//    std::cout << "Norm: " << *vn << std::endl;
}

void Model::loadMTL(const char* Filename){
    std::cout << "Loading " << Filename << std::endl;
    std::ifstream file(Filename);
    std::string line;
    
    char type[7] = "aa";
    Material tmpMat;
    
    while(std::getline(file,line)){  //read line 
        
        sscanf(line.c_str(), "%6s ", type);
        //std::cout << line << std::endl;
        if(!strcmp(type, "newmtl")){
            m_MaterialCount++;
            if(m_MaterialCount > 1){    //Erstes Material wurde komplett ausgelesen
                std::pair<std::string, Material> data(tmpMat.getName(), tmpMat);
                m_pMaterials.insert(data);
            }
            char name[255];
            sscanf(line.c_str(), "%*s %s", name);
            tmpMat.setName(name);
        }else if(!strcmp(type, "Kd")){
            float r, g, b;
            sscanf(line.c_str(), "%*s %f %f %f", &r, &g, &b);
            tmpMat.setDiffuseColor(Color(r,g,b));   
        }else if(!strcmp(type, "Ka")){
            float r, g, b;
            sscanf(line.c_str(), "%*s %f %f %f", &r, &g, &b);
            tmpMat.setAmbientColor(Color(r,g,b));   
        }else if(!strcmp(type, "Ks")){
            float r, g, b;
            sscanf(line.c_str(), "%*s %f %f %f", &r, &g, &b);
            tmpMat.setSpecularColor(Color(r,g,b));   
        }else if(!strcmp(type, "Ns")){
            float exp;
            sscanf(line.c_str(), "%*s %f", &exp);
            tmpMat.setSpecularExponent(exp);   
        }else if(!strcmp(type, "map_Kd")){
            char filename[255];
            sscanf(line.c_str(), "%*s %s", filename);
            std::cout << "Loading Texture " << filename << std::endl;
            tmpMat.setDiffuseTexture(filename);
        }
//        }else if(!strcmp(type, "map_Ks")){
//            char* filename;
//            sscanf(line.c_str(), "%*s %s", filename);
//            tmpMat.setSpecularTexture(filename);
//        }
    }
    
    //Letztes Material einfuegen
    std::pair<std::string, Material> data(tmpMat.getName(), tmpMat);
    m_pMaterials.insert(data);
    
}

bool Model::loadShaders(const char* VertexShader, const char* FragmentShader) {
    shaderLoaded = m_shader.load(VertexShader, FragmentShader);
    return shaderLoaded;
}

//bool Model::loadVertexShader(const char* VertexShader) {
//    shaderLoaded = m_shader.loadVertexShader(VertexShader);
//    return shaderLoaded;
//}
//
//bool Model::loadFragmentShader(const char* FragmentShader) {
//    shaderLoaded = m_shader.loadFragmentShader(FragmentShader);
//    return shaderLoaded;
//}


void Model::buffer(){
    if(!isBuffered){
        glGenBuffers(1, &m_vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, m_VertexCount*sizeof(Vertex), m_pVertices, GL_STATIC_DRAW);       //static draw evtl. aendern

        glGenBuffers(1, &m_indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pIndices.size()*sizeof(GLuint), &m_pIndices[0], GL_STATIC_DRAW);

        //Unbind buffers
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        isBuffered = true;
    }
}

void Model::draw(){
    if(!isBuffered){
        buffer();
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
    
    glPushMatrix();
    
    
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(24));
    

    
    if(shaderLoaded)
        m_shader.activate();
    for(MaterialGroup mg:m_pMGroups){
        //std::cout << "Texturing with " << mg.name << std::endl;
        setMaterial(mg.name);
        glDrawElements(GL_TRIANGLES, mg.count, GL_UNSIGNED_INT, BUFFER_OFFSET(mg.offset));
    }
    if(shaderLoaded)
        m_shader.deactivate();
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    
    glPopMatrix();
    
    //Unbind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

bool Model::setMaterial(std::string matName) {
    Material mat;
    
    try{
        mat = m_pMaterials.at(matName);
    }catch(const std::out_of_range& oor){
        std::cout << "Material " << matName << "not found" << std::endl;
        return false;
    }
    if(shaderLoaded){
        m_shader.setMaterial(mat);
        return true;
    }else
        return false;
}

//void Model::createCube()
//{
//    Vector PositionsFromFile[8] =
//    {
//        Vector(-1, -1, 1),
//        Vector(-1,  1, 1),
//        Vector( 1, -1, 1),
//        Vector( 1,  1, 1),
//        Vector( 1, -1, -1),
//        Vector( 1,  1, -1),
//        Vector(-1, -1, -1),
//        Vector(-1, 1, -1)
//    };
//    struct Texcoord
//    {
//        float s,t;
//    };
//    Texcoord TexcoordFromFile[20] =
//    {
//        { 0, 1 },
//        { 1, 0 },
//        { 0, 0 },
//        { 0, 0 },
//        { 1, 1 },
//        { 0, 1 },
//        { 1, 1 },
//        { 0, 0 },
//        { 1, 0 },
//        { 1, 0 },
//        { 0, 1 },
//        { 1, 1 },
//        { 1, 0 },
//        { 0, 0 },
//        { 1, 1 },
//        { 0, 1 },
//        { 0, 0 },
//        { 1, 0 },
//        { 0, 1 },
//        { 1, 1 }
//    };
//    struct Face
//    {
//        unsigned int pidx[3], tidx[3];
//    };
//    Face FacesFromFile[12]
//    {
//        //face 1
//        {3,4,2,   9, 12, 6},
//        {3,2,1,   9, 6, 3},
//        //face 2
//        {5,6,4,   13,15,11},
//        {5,4,3,   13,11,8},
//        //face 3
//        {7,8,6,   18,20,16},
//        {7,6,5,   18,16,14},
//        //face 4
//        {1,2,8,   2,5,19},
//        {1,8,7,   2,19,17},
//        //face 5
//        {4,6,8,   10,15,19},
//        {4,8,2,   10,19,4},
//        //face 6
//        {5,3,1,   13,7,1},
//        {5,1,7,   13,1,17}
//        
//    };
//    
//    
//    m_pVertices = new Vertex[12*3];
//    m_VertexCount = 12*3;
//    for(int i=0; i<12; i++ )
//    {
//        unsigned int PosIdx0 = FacesFromFile[i].pidx[0]-1;
//        unsigned int PosIdx1 = FacesFromFile[i].pidx[1]-1;
//        unsigned int PosIdx2 = FacesFromFile[i].pidx[2]-1;
//        
//        unsigned int TexIdx0 = FacesFromFile[i].tidx[0]-1;
//        unsigned int TexIdx1 = FacesFromFile[i].tidx[1]-1;
//        unsigned int TexIdx2 = FacesFromFile[i].tidx[2]-1;
//        
//        Vector a = m_pVertices[i*3].Position = PositionsFromFile[ PosIdx0];
//        Vector b = m_pVertices[i*3+1].Position = PositionsFromFile[ PosIdx1];
//        Vector c = m_pVertices[i*3+2].Position = PositionsFromFile[ PosIdx2];
//        
//        m_pVertices[i*3].TexcoordS = TexcoordFromFile[TexIdx0].s;
//        m_pVertices[i*3+1].TexcoordS = TexcoordFromFile[TexIdx1].s;
//        m_pVertices[i*3+2].TexcoordS = TexcoordFromFile[TexIdx2].s;
//        
//        m_pVertices[i*3].TexcoordT = TexcoordFromFile[TexIdx0].t;
//        m_pVertices[i*3+1].TexcoordT = TexcoordFromFile[TexIdx1].t;
//        m_pVertices[i*3+2].TexcoordT = TexcoordFromFile[TexIdx2].t;
//        
//        Vector normal = (b-a).cross(c-a);
//        normal.normalize();
//        
//        m_pVertices[i*3].Normal =
//        m_pVertices[i*3+1].Normal =
//        m_pVertices[i*3+2].Normal = normal;
//    }
//    
//    printf( "Vertices:\n");
//    for( unsigned int i=0; i<m_VertexCount; i++)
//    {
//        printf( "%2i: ", i);
//        printf( "p(%2.1f, %2.1f, %2.1f) ", m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z );
//        printf( "n(%2.1f, %2.1f, %2.1f) ", m_pVertices[i].Normal.X, m_pVertices[i].Normal.Y, m_pVertices[i].Normal.Z );
//        printf( "t(%2.1f, %2.1f)\n", m_pVertices[i].TexcoordS, m_pVertices[i].TexcoordT );
//        
//    }
//}


const BoundingBox& Model::boundingBox() const
{
    return m_Box;
}

void Model::drawLines() const
{
    // Aufgabe 1
}

void Model::drawTriangles() const
{
    // Aufgabe 1
}



