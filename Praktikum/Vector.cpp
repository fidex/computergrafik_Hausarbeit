/* 
 * File:   Vector.cpp
 * Author: phil
 * 
 * Created on 4. August 2015, 14:58
 */

#include "Vector.h"

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




