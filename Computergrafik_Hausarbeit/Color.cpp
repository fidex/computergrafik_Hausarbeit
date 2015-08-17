/* 
 * File:   Color.cpp
 * Author: phil
 * 
 * Created on 10. August 2015, 15:30
 */

#include "Color.h"

Color::Color(): R(0), G(0), B(0) {
}

Color::Color(float r, float g, float b): R(r), G(g), B(b) {
}

Color Color::operator*(const Color& c) const {
    Color tmp(*this);
    tmp.R = tmp.R*c.R;
    tmp.G = tmp.G*c.G;
    tmp.B = tmp.B*c.B;
    return tmp;
}

Color Color::operator*(const float Factor) const{
    Color tmp(*this);
    tmp.R = tmp.R*Factor;
    tmp.G = tmp.G*Factor;
    tmp.B = tmp.B*Factor;
    return tmp;
}

Color& Color::operator+=(const Color& c) {
    this->R += c.R;
    this->G += c.G;
    this->B += c.B;
    return *this;
}

Color Color::operator+(const Color& c) const {
    Color tmp(*this);
    return tmp += c;
}




