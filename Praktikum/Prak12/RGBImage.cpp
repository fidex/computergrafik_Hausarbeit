/* 
 * File:   RGBImage.cpp
 * Author: phil
 * 
 * Created on 10. August 2015, 15:44
 */

#include "RGBImage.h"
#include <algorithm>
#include <fstream>


RGBImage::RGBImage(unsigned int Width, unsigned int Height): m_Height(Height), m_Width(Width) {
    m_Image = new Color[Height*Width];
//    for (int i = 0; i < Height; i++) {
//        m_Image[i] = new Color[Width]();
//    }
//    m_Image = new Color[Height][Width];
}


RGBImage::RGBImage(const RGBImage& orig) {
    m_Height = orig.height();
    m_Width = orig.width();
    m_Image = new Color[orig.height()*orig.width()];
    std::copy(orig.m_Image, orig.m_Image + (orig.m_Height*orig.m_Width), m_Image);
}

RGBImage::~RGBImage() {
    delete[] m_Image;
}

RGBImage& RGBImage::operator=(RGBImage other) {
    swap(*this, other); // (2)

    return *this;
}

void swap(RGBImage& first, RGBImage& second) {
    std::swap(first.m_Height, second.m_Height);
    std::swap(first.m_Width, second.m_Width);
    std::swap(first.m_Image, second.m_Image);
}


unsigned int RGBImage::height() const {
    return this->m_Height;
}

unsigned int RGBImage::width() const {
    return this->m_Width;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const {        //TODO: Randbehandlung
    if(x >= this->m_Width || y >= this->m_Height) throw 1;
    return m_Image[m_Width*y+x];
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {      //^
    if(x >= this->m_Width || y >= this->m_Height) return;
    m_Image[m_Width*y+x] = c; //Memory Leak?
}

typedef unsigned long DWORD;
typedef unsigned short WORD;

struct BITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
};

struct BITMAPINFOHEADER {
    DWORD biSize;
    DWORD biWidth;
    DWORD biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    DWORD biXPelsPerMeter;
    DWORD biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

bool RGBImage::saveToDisk(const char* Filename) {
    std::ofstream fs;
    fs.open (Filename);
    
    BITMAPFILEHEADER fileHeader;
    fileHeader.bfType = 0x4d42;
    fileHeader.bfSize = 14 + 40 + (this->m_Width*this->m_Height*3) + ((this->m_Width*3)%4)*this->m_Height;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = 14 + 40;
    
    BITMAPINFOHEADER fileInfoHeader;
    fileInfoHeader.biSize = 40;
    fileInfoHeader.biWidth = this->m_Width;
    fileInfoHeader.biHeight = this->m_Height;
    fileInfoHeader.biPlanes = 1;
    fileInfoHeader.biBitCount = 24;
    fileInfoHeader.biCompression = 0;
    fileInfoHeader.biSizeImage = 0;
    fileInfoHeader.biXPelsPerMeter = 0;
    fileInfoHeader.biYPelsPerMeter = 0;
    fileInfoHeader.biClrUsed = 0;
    fileInfoHeader.biClrImportant = 0;
    
    //std::cout << sizeof(WORD) << " - " << sizeof(long) << std::endl;
    
    fs.write(reinterpret_cast<char*>(&fileHeader.bfType), 2);
    fs.write(reinterpret_cast<char*>(&fileHeader.bfSize), 4);
    fs.write(reinterpret_cast<char*>(&fileHeader.bfReserved1), 2);
    fs.write(reinterpret_cast<char*>(&fileHeader.bfReserved2), 2);
    fs.write(reinterpret_cast<char*>(&fileHeader.bfOffBits), 4);
    
    
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biSize), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biWidth), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biHeight), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biPlanes), 2);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biBitCount), 2);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biCompression), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biSizeImage), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biXPelsPerMeter), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biYPelsPerMeter), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biClrUsed), 4);
    fs.write(reinterpret_cast<char*>(&fileInfoHeader.biClrImportant), 4);
    
    for(int y = this->m_Height-1; y >= 0; y--){
        for(int x = 0; x < this->m_Width; x++){
            Color c = this->getPixelColor(x, y);
            fs << convertColorChannel(c.B);
            fs << convertColorChannel(c.G);
            fs << convertColorChannel(c.R);
        }
        for(int k = 0; k < this->m_Width%4; k++){
            char n[1] = {0};
            fs.write(n,1);
        }
    }
    
    fs.close();
    return true;
}

