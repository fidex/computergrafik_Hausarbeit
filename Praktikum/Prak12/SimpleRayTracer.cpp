/* 
 * File:   SimpleRayTracer.cpp
 * Author: phil
 * 
 * Created on 13. August 2015, 15:33
 */

#include "SimpleRayTracer.h"
#include <math.h>

SimpleRayTracer::SimpleRayTracer(unsigned int maxDepth): m_MaxDepth(maxDepth), m_Cam(Camera(-8.0, 1.0, 1.0, 0.75, 640, 480)) {}

Color SimpleRayTracer::localIllumination(const Vector& SurfacePoint, const Vector& Eye, const Vector& Normal, const PointLight& Light, const Material& Material, bool shadow) {
    //    Color c(0,0,0);
    Vector N = Normal;
    N.normalize();
    Vector L = Light.Position-SurfacePoint;
    Color l = Light.Intensity;

    Vector E = Eye-SurfacePoint;
    E.normalize();

    Vector H = (E+L) * 0.5;
    H.normalize();
    L.normalize();
    
    //AMBIENT
    Color c = Material.getAmbientCoeff(SurfacePoint);
    
    if(!shadow){
        //DIFFUSE
        c +=(l*Material.getDiffuseCoeff(SurfacePoint))*fmax(0.0,N.dot(L));
        //printf( "l%1.2f ",ndotl);
            //SPECULAR
        c += (l*Material.getSpecularCoeff(SurfacePoint)) * pow(fmax(0.0,N.dot(H)), Material.getSpecularExp(SurfacePoint));
        //printf( "h%1.2f ",ndoth);
    }
    return c;
}

void SimpleRayTracer::traceScene(const Scene& SceneModel, RGBImage& Image) {
    for(int x = 0; x < Image.width(); x++){
        for(int y = 0; y < Image.height(); y++){
            Image.setPixelColor(x, y, trace(SceneModel, m_Cam.Position(), m_Cam.generateRay(x, y), 1));
        }
    }
}

Color SimpleRayTracer::trace(const Scene& SceneModel, const Vector& o, const Vector& d, int depth) {
    Color c(0,0,0);
    
    bool hit = false;
    Triangle tHit;
    
    float s = 0;
    float oldS = 0;
    for(int i = 0; i < SceneModel.getTriangleCount(); i++){
        const Triangle triangle = SceneModel.getTriangle(i);
        if(o.triangleIntersection(d, triangle.A, triangle.B, triangle.C, s)){
            if((s <= oldS && s > 0.1) || oldS == 0){
                hit = true;
                tHit = SceneModel.getTriangle(i);
                oldS = s;
            }
        }
    }
    
    if (!hit){
        return c;
    }
    
    Vector hit_point = o + d*oldS;

    
    for(int i = 0; i < SceneModel.getLightCount(); i++){
        if(isVisible(SceneModel, hit_point, SceneModel.getLight(i)))
            c += localIllumination(hit_point, o, tHit.calcNormal(hit_point), SceneModel.getLight(i), *tHit.pMtrl, false);
        else
            c += localIllumination(hit_point, o, tHit.calcNormal(hit_point), SceneModel.getLight(i), *tHit.pMtrl, true);
    }
    
    if(this->m_MaxDepth < depth+1){
        return c;
    }
    return c + (trace(SceneModel, hit_point, d.reflection(tHit.calcNormal(hit_point)), depth+1) * tHit.pMtrl->getReflectivity(hit_point));
}

bool SimpleRayTracer::isVisible(const Scene &SceneModel, Vector hit_point, PointLight p){
    float s = 0;
    
    Vector d = (p.Position - hit_point).normalize();
    float distance = (p.Position - hit_point).length();
    
    for(int i = 0; i < SceneModel.getTriangleCount(); i++){
        const Triangle triangle = SceneModel.getTriangle(i);
        if(hit_point.triangleIntersection(d, triangle.A, triangle.B, triangle.C, s)){
            if((s < distance && s > 0.1)){
                return false;
            }
        }
    }
    return true;
    
}





