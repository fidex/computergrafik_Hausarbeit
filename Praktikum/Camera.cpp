/* 
 * File:   Camera.cpp
 * Author: phil
 * 
 * Created on 12. August 2015, 17:41
 */

#include "Camera.h"

Camera::Camera(float zvalue, float planedist, float width, float height, 
                unsigned int widthInPixel, unsigned int heightInPixel): zvalue(zvalue), planedist(planedist), 
                                                                        width(width), height(height),
                                                                        widthInPixel(widthInPixel), heightInPixel(heightInPixel)
{}

Vector Camera::Position() const {
    return Vector(0,0,zvalue);
}

Vector Camera::generateRay(unsigned int x, unsigned int y) const {
    float a = -(width /2 - width /widthInPixel *x);
    float b =   height/2 - height/heightInPixel*y;                                                
    return Vector(a,b,planedist).normalize();
}


