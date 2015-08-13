/* 
 * File:   Color.h
 * Author: phil
 *
 * Created on 10. August 2015, 15:30
 */

#ifndef COLOR_H
#define	COLOR_H

class Color {
public:
    float R;
    float G;
    float B;
    
    Color();
    Color(float r, float g, float b);
    Color operator*(const Color& c) const;
    Color operator*(const float Factor) const;
    Color operator+(const Color& c) const;
    Color& operator+=(const Color& c);

};

#endif	/* COLOR_H */

