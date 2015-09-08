/* 
 * File:   Ground.cpp
 * Author: phil
 * 
 * Created on 7. September 2015, 21:09
 */

#include "Ground.h"
#include "ShaderProgram.h"
#include <stdint.h>
#include <fstream>
#include <vector>

#define DWORD uint32_t
#define WORD __uint16_t
#define LONG int32_t
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void printVector(const Vector& v){
    std::cout << "x:" << v.X << " y:" << v.Y << " z:" << v.Z << std::endl;
}

Ground::Ground() {

}


Ground::Ground(GLuint XSize, GLuint YSize, GLuint maxHeight):m_XSize(XSize), m_ZSize(YSize), m_MaxHeight(maxHeight)
{
}

Ground::~Ground() {
    if(m_Vertices != NULL){
        delete[] m_Vertices;
    }
}


void Ground::loadHeightmap(const char* filename) {
    loadBMP(filename);
}

void Ground::loadShaders(const char* vertexShader, const char* fragmentShader) {
    m_Shader.load(vertexShader, fragmentShader);
    shaderLoaded = true;
}

//Src: https://en.wikipedia.org/wiki/Linear_interpolation
//float lerp(float v0, float v1, float t) {
//  return (1-t)*v0 + t*v1;
//}

// Source: http://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
void Ground::loadBMP(const char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");

    if(f == NULL){
        std::cout << "Heightmap not found: " << filename << std::endl;
        return;
    }
        
        //throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    std::cout << std::endl;
    std::cout << "  Name: " << filename << std::endl;
    std::cout << " Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    //GLuint* pixelValues = new GLuint[width*height];
    GLfloat** pixelValues = new GLfloat*[height];
    GLfloat val;
    for(int i = 0; i < height; i++)
    {
        pixelValues[i] = new GLfloat[width];
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)

//            tmp = data[j];
//            data[j] = data[j+2];
//            data[j+2] = tmp;
            //std::cout << (float) data[j] << std::endl;
            val = (data[j]+data[j+1]+data[j+2])/3;
            
            pixelValues[i][j/3] = interpolateRGB(val, 0, m_MaxHeight);
//            std::cout << pixelValues[i][j/3] << std::endl;
            //pixelValues[(j/3)*width+i] = lerp(0.0f, m_MaxHeight, (data[j]+data[j+1]+data[j+2])/3);
           // std::cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< std::endl;
        }
    }
    delete[] data;
    fclose(f);
    
    //Converting to Quads and Indexing
    //@todo Performanceloch, vesuchen in eine for zu stopfen!
    
    std::unordered_map<Vertex, GLuint, VertexHasher> vertexMap;
    Vertex tmp1, tmp2, tmp3, tmp4;
    std::pair<std::unordered_map<Vertex, GLuint, VertexHasher>::iterator, bool> indexData1, indexData2, indexData3, indexData4;
    GLuint vertexcounter = 0;
    GLuint xCoord, xPlusCoord, zCoord, zPlusCoord;
    for(int x=0; x<width-1;x++){
        for(int z=0; z<height-1;z++){
            //std::cout << x << " " << z << std::endl;
//            tmp1.Position = Vector(x, pixelValues[z*m_Width+x], z).normalize();
//            tmp2.Position = Vector(x+1, pixelValues[z*m_Width+(x+1)], z).normalize();
//            tmp3.Position = Vector(x, pixelValues[(z+1)*m_Width+x], z+1).normalize();
//            tmp4.Position = Vector(x+1, pixelValues[(z+1)*m_Width+(x+1)], z+1).normalize();
            
//            xCoord = interpolate(x, 0, width-2, 0, m_XSize);
//            xPlusCoord = interpolate(x+1, 0, width-2, 0, m_XSize);
//            zCoord = interpolate(z, 0, height-2, 0, m_ZSize);
//            zPlusCoord = interpolate(z+1, 0, height-2, 0, m_ZSize);
//            tmp1.Position = Vector(xCoord, pixelValues[z][x], zCoord);
//            tmp2.Position = Vector(xPlusCoord, pixelValues[z][x+1], zCoord);
//            tmp3.Position = Vector(xCoord, pixelValues[z+1][x], zPlusCoord);
//            tmp4.Position = Vector(xPlusCoord, pixelValues[z+1][x+1], zPlusCoord);
            
            tmp1.Position = Vector(x, pixelValues[z][x], z);
            tmp2.Position = Vector(x, pixelValues[z+1][x], z+1);
            tmp3.Position = Vector(x+1, pixelValues[z+1][x+1], z+1);
            tmp4.Position = Vector(x+1, pixelValues[z][x+1], z);
            
//            std::cout << "Quad:" << std::endl;
//            printVector(tmp1.Position);
//            printVector(tmp2.Position);
//            printVector(tmp3.Position);
//            printVector(tmp4.Position);
            
            indexData1 = vertexMap.insert(std::make_pair(tmp1, vertexcounter));

            indexData2 = vertexMap.insert(std::make_pair(tmp2, vertexcounter+1));

            indexData3 = vertexMap.insert(std::make_pair(tmp3, vertexcounter+2));

            indexData4 = vertexMap.insert(std::make_pair(tmp4, vertexcounter+3));
            if(indexData1.second){
                m_Indices.push_back(vertexcounter);
                vertexcounter++;
            }else{
                m_Indices.push_back(indexData1.first->second);
            }
            if(indexData2.second){
                m_Indices.push_back(vertexcounter);
                vertexcounter++;
            }else{
                m_Indices.push_back(indexData2.first->second);
            }
            if(indexData3.second){
                m_Indices.push_back(vertexcounter);
                vertexcounter++;
            }else{
                m_Indices.push_back(indexData3.first->second);
            }
            if(indexData4.second){
                m_Indices.push_back(vertexcounter);
                vertexcounter++;
            }else{
                m_Indices.push_back(indexData4.first->second);
            }
        }
    }
    delete[] pixelValues;
    m_VertexCount = vertexMap.size();
    m_Vertices = new Vertex[vertexMap.size()];
    for(auto itr:vertexMap){
        m_Vertices[itr.second] = itr.first;
    }
    

}



void Ground::buffer() {
        if(!isBuffered){
            std::cout << "Buffering Ground" << std::endl;
            glGenBuffers(1, &m_VBID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBID);
            glBufferData(GL_ARRAY_BUFFER, m_VertexCount*sizeof(Vertex), m_Vertices, GL_STATIC_DRAW);       //static draw evtl. aendern

            glGenBuffers(1, &m_IBID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size()*sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

            //Unbind buffers
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            isBuffered = true;
        }
    
}


void Ground::draw() {
    if(!isBuffered){
        buffer();
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBID);

    
    glPushMatrix();
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    //std::cout << "Drawing " << m_Indices.size() << std::endl;
//    glDrawArrays(GL_TRIANGLES, 0, m_PixelCount);
    //glColor3f(1.0, 0.0, 0.0);
    if(shaderLoaded){
        m_Shader.activate();
    }
    glDrawElements(GL_QUADS, m_Indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    if(shaderLoaded){
        m_Shader.deactivate();
    }
    
    glDisableVertexAttribArray(0);
    
    glPopMatrix();
    
    //Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Ground::getMaxHeight() const {
    return m_MaxHeight;
}

void Ground::setMaxHeight(GLuint maxHeight)  {
        this->m_MaxHeight = maxHeight;
}

GLfloat Ground::interpolateRGB(GLfloat inVal, GLfloat min, GLfloat max) {
    return interpolate(inVal, 0, 255, min, max);
}


GLfloat Ground::interpolate(GLfloat inVal, GLfloat minInRange, GLfloat maxInRange, GLfloat minOutRange, GLfloat maxOutRange) {
    GLfloat x = inVal / (maxInRange - minInRange);
    return minOutRange + (maxOutRange - minOutRange) * x;
}
