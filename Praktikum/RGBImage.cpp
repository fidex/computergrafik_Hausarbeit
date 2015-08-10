/* 
 * File:   RGBImage.cpp
 * Author: phil
 * 
 * Created on 10. August 2015, 15:44
 */

#include "RGBImage.h"
#include <algorithm>


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
    return m_Image[m_Width*y+x];
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {      //^
    m_Image[m_Width*y+x] = c; //Memory Leak?
}

unsigned char RGBImage::convertColorChannel(float f) {

}

bool RGBImage::saveToDisk(const char* Filename) {

}

