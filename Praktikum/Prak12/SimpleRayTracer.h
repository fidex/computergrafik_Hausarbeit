/* 
 * File:   SimpleRayTracer.h
 * Author: phil
 *
 * Created on 13. August 2015, 15:33
 */

#ifndef SIMPLERAYTRACER_H
#define	SIMPLERAYTRACER_H

#include "cgutilities/CGUtilities.h"
#include "RGBImage.h"
#include "Vector.h"
#include "Color.h"
#include "Camera.h"

class SimpleRayTracer {
public:
    SimpleRayTracer(unsigned int maxDepth);
    void traceScene(const Scene& SceneModel, RGBImage& Image);
    
protected:
    Color trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth);
    Color localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight& Light, const Material& Material, bool shadow);
    int m_MaxDepth;
    Camera m_Cam;
private:
    bool isVisible(const Scene &SceneModel, Vector hit_point, PointLight p);
};

#endif	/* SIMPLERAYTRACER_H */

