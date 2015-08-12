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

/*
 * 
 */
int main(int argc, char** argv) {
    
    Test1::vector();
    
    Test2::color();
    
    Test3::rgbimage("/home/phil/uni/cg/test/");
    return 0;
}

