/* 
 * File:   Camera.h
 * Author: phil
 *
 * Created on 12. August 2015, 17:41
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "Vector.h"

class Camera {
public:
    Camera(float zvalue, float planedist, float width, float height, unsigned int widthInPixel, unsigned int heightInPixel);
    Vector generateRay(unsigned int x, unsigned int y) const;
    Vector Position() const;
private:
    float zvalue;
    float planedist;
    float width;
    float height;
    unsigned int widthInPixel;
    unsigned int heightInPixel;
};

#endif	/* CAMERA_H */

