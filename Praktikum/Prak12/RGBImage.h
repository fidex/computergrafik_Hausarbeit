/* 
 * File:   RGBImage.h
 * Author: phil
 *
 * Created on 10. August 2015, 15:44
 */

#ifndef RGBIMAGE_H
#define	RGBIMAGE_H

#include "Color.h"

class RGBImage {
public:
    RGBImage(unsigned int Width, unsigned int Height);
    RGBImage(const RGBImage& orig);
    ~RGBImage();
    RGBImage & operator=(RGBImage other);
    void setPixelColor(unsigned int x, unsigned int y, const Color& c);
    const Color& getPixelColor(unsigned int x, unsigned int y) const;
    bool saveToDisk(const char* Filename);
    unsigned int width() const;
    unsigned int height() const;
    friend void swap(RGBImage& first, RGBImage& second);

    static unsigned char convertColorChannel(float f){
        if (f > 1.0) return (unsigned char)255;
        if (f < 0.0) return (unsigned char)0;
        return (unsigned char)(f * 255.0);
    }
protected:
    Color* m_Image;
    unsigned int m_Height;
    unsigned int m_Width;
};

#endif	/* RGBIMAGE_H */

