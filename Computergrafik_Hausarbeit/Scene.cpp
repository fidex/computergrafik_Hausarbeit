/* 
 * File:   Scene.cpp
 * Author: phil
 * 
 * Created on 8. September 2015, 13:24
 */

#include "Scene.h"
#include "Constants.h"

Scene::Scene() {
}

Ground Scene::GetTerrain() const {
    return m_Terrain;
}

void Scene::SetTerrain(Ground Terrain) {
    m_Terrain = Terrain;
}

Object* Scene::GetObject(std::string name) {
    try{
        return m_ObjRoot.findObject(name);
    }catch(std::out_of_range& oor){
        return NULL;
    }
}

void Scene::addModel(Model m) {
    m_Models.insert(std::make_pair(m.getName(), m));
}

void Scene::addObject(Object obj) {
    m_ObjRoot.addChildObject(obj);
}

void Scene::drawScene() {

    m_Terrain.draw();
    m_ObjRoot.draw();
    
}

void Scene::readSceneFile(const char* filename) {
    StringBlockGen reader(filename, "}");
    std::vector<std::string> currBlock;
    while((currBlock = reader.getNextBlock()).size() > 0){
        parseSceneBlock(currBlock);
    }
}

void Scene::parseSceneBlock(std::vector<std::string>& block) {
    //Skip empty lines between blocks
    int linenumber=0;
    std::vector<std::string>::iterator itr = block.begin();
    while(itr->length() == 0){
        itr++;
    }
    block.erase(block.begin(),itr);
    
    if(!block[0].compare(0, 7,"terrain")){
        parseTerrainBlock(block);
    }else if(!block[0].compare(0, 5,"model")){
        parseModelBlock(block);
    }else if(!block[0].compare(0, 6,"object")){
        parseObjectBlock(block);
    }
}

void ltrim(std::string& str){
    //str.erase(str.find_last_not_of(" \n\r\t")+1);
    str.erase(0, str.find_first_not_of(" \n\r\t"));
}

void Scene::parseTerrainBlock(std::vector<std::string>& block) {
    char filename[255];
    for(std::string line:block){
        ltrim(line);
        if(!line.compare(0, 4,"size")){
            int XSize, ZSize, MaxHeight;
            sscanf(line.c_str(), "%*s %d %d %d", &XSize, &MaxHeight, &ZSize);
            m_Terrain.setXSize(XSize);
            m_Terrain.setZSize(ZSize);
            m_Terrain.setMaxHeight(MaxHeight);
        }else if(!line.compare(0, 9,"heightmap")){
            sscanf(line.c_str(), "%*s %s", filename);
        }else if(!line.compare(0, 6,"mixmap")){
            //Load mixmap
        }else if(!line.compare(0, 10,"detailmap1")){
            float u,v;
            sscanf(line.c_str(), "%*s %*s %f %f", &u, &v);
            m_Terrain.setDM1TillingU(u);
            m_Terrain.setDM1TillingV(v);
            //Load detailmap1
        }else if(!line.compare(0, 10,"detailmap2")){
            //Load detailmap2
        }
        
    }
    m_Terrain.loadHeightmap(filename);
    

    //load Terrain Shader
    m_Terrain.loadShaders(g_TerrainVShader, g_TerrainFShader);
    
}

void Scene::parseModelBlock(std::vector<std::string>& block) {
//    Model m;
    char name[255];
    sscanf(block[0].c_str(), "%*s %s ", name);
    std::string cpname(name);
    if(cpname.back() == '{'){
        cpname.erase(cpname.end()-1);
    }
    m_Models[cpname].setName(cpname);
    std::cout << "Parsing Model " << name << std::endl;
    for(std::string line:block){
        ltrim(line);
        if(!line.compare(0, 4,"file")){
            char filename[255];
            sscanf(line.c_str(), "%*s %s", filename);
            if(!m_Models[cpname].loadOBJ(filename)){
                std::cout << "Error loading Model " << name << std::endl;
                return;
            }
        }
    }
    
    m_Models[cpname].loadShaders(g_ModelVShader, g_ModelFShader);
    
//    m_Models.insert(std::make_pair(std::string(name), m));
}

void Scene::parseObjectBlock(std::vector<std::string>& block) {
    Object obj;
    char name[255];
    sscanf(block[0].c_str(), "%*s %s[{\n]", name);
    std::string cpname(name);
    if(cpname.back() == '{'){
        cpname.erase(cpname.end()-1);
    }
    obj.SetName(std::string(cpname));
    std::cout << "Parsing Object " << cpname << std::endl;
    for(std::string line:block){
        ltrim(line);
        if(!line.compare(0, 11,"translation")){
            Vector v;
            sscanf(line.c_str(), "%*s %f %f %f", &v.X, &v.Y, &v.Z);
            obj.SetTranslation(v);
        }else if(!line.compare(0, 8,"rotation")){
            Vector v;
            float angle;
            sscanf(line.c_str(), "%*s %f %f %f %f", &v.X, &v.Y, &v.Z, &angle);
            obj.SetRotationAngle(angle);
            obj.SetRotationAxis(v);
        }else if(!line.compare(0, 7,"scaling")){
            Vector v;
            sscanf(line.c_str(), "%*s %f %f %f", &v.X, &v.Y, &v.Z);
            obj.SetScaling(v);
        }else if(!line.compare(0, 5,"model")){
            char modelName[255];
            sscanf(line.c_str(), "%*s %s", modelName);
            try{
                obj.SetModel(&m_Models.at(modelName));
            }catch(std::out_of_range& oor){
                std::cout << "Failed to set Model " << modelName << std::endl 
                        << "Make sure to define Models first!" << std::endl;
            }
        }else if(!line.compare(0, 6,"parent")){
            char parentName[255];
            sscanf(line.c_str(), "%*s %s", parentName);
            if(!strcmp(parentName, "NULL")){
                obj.SetParentName("NULL");
                m_ObjRoot.addChildObject(obj);
            }else{
                std::string name(parentName);
                obj.SetParentName(name);
                if(!m_ObjRoot.adoption(name, obj)){
                    std::cout << "Couldn't find parent " << parentName << ". Poor " << obj.GetName() << "." << std::endl;
                }
            }
        }
    }
}

void Scene::saveSceneFile(const char* filename) {
    std::ofstream  file;
    file.open(filename);
    file << m_Terrain.toString();
    for (auto it=m_Models.begin(); it!=m_Models.end(); ++it){
        file << it->second.toString();
    }
    std::vector<Object> objects;
    m_ObjRoot.getAllObjects(objects);
    for(Object obj:objects){
        file << obj.toString();
    }
    file.close();  
}

