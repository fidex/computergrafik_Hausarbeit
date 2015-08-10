/* 
 * File:   Vector.cpp
 * Author: phil
 * 
 * Created on 4. August 2015, 14:58
 */

#include "Vector.h"
#include <cmath>

Vector::Vector() {
}

Vector::Vector(float x, float y, float z): X(x),
                                           Y(y),
                                           Z(z){}

Vector Vector::cross(const Vector& v) const {
    Vector nv = Vector();
    nv.X = (this->Y*v.Z) - (this->Z*v.Y);
    nv.Y = (this->Z*v.X) - (this->X*v.Z);
    nv.Z = (this->X*v.Y) - (this->Y*v.X);
    return nv;
}

float Vector::dot(const Vector& v) const {
    return (this->X*v.X)+(this->Y*v.Y)+(this->Z*v.Z);
}

Vector& Vector::normalize(){
    float length = this->length();
    this->X = this->X/length;
    this->Y = this->Y/length;
    this->Z = this->Z/length;
    return *this;
}

float Vector::length() const{
    return sqrt(this->lengthSquared());
}

float Vector::lengthSquared() const{
    return pow(this->X,2)+pow(this->Y,2)+pow(this->Z,2);
}

Vector Vector::operator+(const Vector& v) const {
    Vector tmp(*this);
    return tmp += v;
}

Vector Vector::operator-(const Vector& v) const {
    Vector tmp(*this);
    return tmp -= v;
}

Vector& Vector::operator+=(const Vector& v) {
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this;
}

Vector& Vector::operator-=(const Vector& v) {
    this->X -= v.X;
    this->Y -= v.Y;
    this->Z -= v.Z;
    return *this;
}


Vector Vector::operator*(float c) const {
    return Vector(this->X*c, this->Y*c, this->Z*c);
}


Vector Vector::reflection(const Vector& normal) const {
    Vector refl;
    normal.normalize();
    refl = *this - (normal*2.0*this->dot(normal));
    return refl;
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const {
    
}
