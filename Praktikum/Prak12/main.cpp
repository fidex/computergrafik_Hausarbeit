/* 
 * File:   main.cpp
 * Author: fide
 *
 * Created on 4. August 2015, 15:50
 */

#include <cstdlib>
#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "RGBImage.h"
#include "SimpleRayTracer.h"
#include "cgutilities/CGUtilities.h"

/*
 * 
 */
int main(int argc, char** argv) {
    
    
/* A1 *//*
    Test1::vector();
    
    Test2::color();
    
    Test3::rgbimage("/home/phil/uni/cg/test/");
    return 0;
         */
    Scene ModelScene(20);
    RGBImage Image(640,480);
    SimpleRayTracer Raytracer(2);
    Raytracer.traceScene(ModelScene, Image);
    Image.saveToDisk("raytracing_image.bmp");
}

