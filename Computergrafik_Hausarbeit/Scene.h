/* 
 * File:   Scene.h
 * Author: phil
 *
 * Created on 8. September 2015, 13:23
 */

#ifndef SCENE_H
#define	SCENE_H

#include <unordered_map>
#include <cstring>
#include "Ground.h"
#include "Model.h"
#include "Object.h"
#include "StringBlockGen.h"

class Scene {
public:
    Scene();
    
    Ground GetTerrain() const;
    void SetTerrain(Ground Terrain);
    Object* GetObject(std::string name);
    
    void readSceneFile(const char* filename);
    void saveSceneFile(const char* filename);
    
    
    void addModel(Model m);
    void addObject(Object obj);
    
    void drawScene();

    
private:
    void parseSceneBlock(std::vector<std::string>& block);
    void parseTerrainBlock(std::vector<std::string>& block);
    void parseModelBlock(std::vector<std::string>& block);
    void parseObjectBlock(std::vector<std::string>& block);
    
    Ground m_Terrain;
    std::unordered_map<std::string, Model> m_Models;
    Object m_ObjRoot;
    
};

#endif	/* SCENE_H */

