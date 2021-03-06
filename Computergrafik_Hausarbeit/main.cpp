//
//  main.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include <pthread.h>

#include "ShaderProgram.h"
#include "Ground.h"
#include "Object.h"
#include "Scene.h"

// Model that should be loaded
const char* g_ModelToLoad = "testkiste2.obj";
const char* g_HeightmapToLoad = "sack_rgb.bmp";

// window x and y size
const unsigned int g_WindowWidth=640;
const unsigned int g_WindowHeight=480;

// light position (point light)
const Vector g_LightPos = Vector( 0,4,0);

std::string g_path("./shader/");
ShaderProgram g_Shader;




Camera g_Camera;
Model g_Model;
Ground g_Ground(10, 10, 5);
Object g_Obj;
Scene g_Scene;

int g_MouseButton = 0;
int g_MouseState = 0;

void SetupDefaultGLSettings();
void DrawScene();
void drawLineTest(Vector v,Vector v2);
void drawTriangleTest(Vector v1, Vector v2, Vector v3);
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);

enum RenderMode
{
    RENDERMODE_LINES,
    RENDERMODE_TRIANGLES,
    LAST_RENDERMODE
};

RenderMode g_RenderMode = RENDERMODE_TRIANGLES;

int main(int argc, char * argv[])
{
    std::cout << "Test" << std::endl;
    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CG Praktikum");
    
    GLenum err = glewInit();
    if(GLEW_OK != err){
        std::cout << "Glew Failed" << std::endl;
    }
            
    
    SetupDefaultGLSettings();
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMotionFunc(MouseMoveCallback);
    
//    ShaderProgram g_Shader;
//    g_Shader.load((g_path+"toon.vert").c_str(), (g_path+"toon.frag").c_str());
//    g_Shader.activate();

    
//    g_Model.loadOBJ(g_ModelToLoad);
//    g_Model.loadShaders((g_path+"test.vert").c_str(), (g_path+"test.frag").c_str());
//    g_Obj.SetModel(&g_Model);
//    g_Obj.SetRotationAngle(30.0);
//    g_Obj.SetRotationAxis(Vector(1,0,0));
//    g_Obj.SetTranslation(Vector(-2, 0, 0));
//    g_Obj.SetScaling(Vector(1,1,1));
//    g_Ground.loadHeightmap(g_HeightmapToLoad);
//    g_Ground.loadShaders((g_path+"test.vert").c_str(), (g_path+"test.frag").c_str());
    
    g_Scene.readSceneFile("test.scene");
    //GLuint test = 0-1;
    //std::cout << test << std::endl;
    glutMainLoop();
    
}


void SetupDefaultGLSettings()
{
    glClearColor(0, 0, 0, 200);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (double)g_WindowWidth/(double)g_WindowHeight, 0.045f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Setup Light Color
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Setup default material
    float diff[4] = {1,1,1,1};
    float amb[4]  = {0.2f,0.2f,0.2f,1};
    float spec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void DrawGroundGrid()
{
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for( unsigned int i=0; i<GridSegments+1; i++)
        {
            float itpos=GridOrigin + GridStep*(float)i;
            glVertex3f(itpos, 0, GridOrigin);
            glVertex3f(itpos, 0, GridOrigin+GridSize);
        
            glVertex3f(GridOrigin, 0, itpos);
            glVertex3f(GridOrigin+GridSize, 0, itpos);

        }
    glEnd();
    glEnable( GL_LIGHTING);

}

void MouseCallback(int Button, int State, int x, int y)
{
    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);
}

void MouseMoveCallback( int x, int y)
{
    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

void KeyboardCallback( unsigned char key, int x, int y)
{
    if( key == 'l')
        g_RenderMode=RENDERMODE_LINES;
    else if( key == 't')
        g_RenderMode=RENDERMODE_TRIANGLES;
    
}


void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    g_Camera.apply();
    
    DrawGroundGrid();
    
    /* tests*/
//    Vector v = Vector(1.0f,1.0f,1.0f);
//    Vector v2 = Vector(2.0f,2.0f,3.0f);
//    drawLineTest(v,v2);
//    
//    drawTriangleTest(Vector(1.0f,1.0f,1.0f),Vector(2.0f,2.0f,2.0f), Vector(3.0f,2.0f,3.0f));
//    
//    GLfloat lpos[4];
//    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
//    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
//    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lpos );
//    if(g_RenderMode == RENDERMODE_LINES)
//    {
//        glDisable(GL_LIGHTING);
//        g_Model.draw();
//        glEnable(GL_LIGHTING);
//    }
//    else if(g_RenderMode== RENDERMODE_TRIANGLES){
            
//    }
//    g_Ground.draw();
//    //g_Model.draw();
//    g_Obj.draw();
    
    g_Scene.drawScene();
    
    glutSwapBuffers();
    glutPostRedisplay();
    
}

void drawLineTest(Vector v1,Vector v2){
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 0.0f);        
        
            glVertex3f(v1.X, v1.Z, v1.Y);
            glVertex3f(v2.X, v2.Z, v2.Y);       
        
    glEnd();
    glEnable( GL_LIGHTING);
    
    
}
void drawTriangleTest(Vector v1, Vector v2, Vector v3){
    glDisable( GL_LIGHTING);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);        
        
            glVertex3f(v1.X, v1.Z, v1.Y);
            glVertex3f(v2.X, v2.Z, v2.Y);
            glVertex3f(v3.X, v3.Z, v3.Y);           
        
    glEnd();
    glEnable( GL_LIGHTING);
}

void junk() {
  int i;
  i=pthread_getconcurrency();
};