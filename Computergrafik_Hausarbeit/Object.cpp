/* 
 * File:   Object.cpp
 * Author: phil
 * 
 * Created on 7. September 2015, 20:09
 */

#include "Object.h"

Object::Object() {
}

Object::Object(std::string name, Vector translation, Vector scaling, Vector rotAxis, GLfloat rotAngle):m_Name(name), 
                                                                                                        m_Translation(translation), 
                                                                                                        m_RotationAxis(rotAxis),
                                                                                                        m_RotationAngle(rotAngle),
                                                                                                        m_Scaling(scaling)
{}                                                                                                                                                      


std::string Object::GetName() const {
    return m_Name;
}

GLfloat Object::GetRotationAngle() const {
    return m_RotationAngle;
}

const Vector& Object::GetRotationAxis() const {
    return m_RotationAxis;
}

const Vector& Object::GetTranslation() const {
    return m_Translation;
}

void Object::SetName(std::string Name) {
    m_Name = Name;
}

void Object::SetRotationAngle(GLfloat RotationAngle) {
    m_RotationAngle = RotationAngle;
    for(auto itr:m_Children){
        itr.second.SetRotationAngle(RotationAngle);
    }
}

void Object::SetRotationAxis(Vector RotationAxis) {
    m_RotationAxis = RotationAxis;
    for(auto itr:m_Children){
        itr.second.SetRotationAxis(RotationAxis);
    }
}

void Object::SetTranslation(Vector Translation) {
    m_Translation = Translation;
    for(auto itr:m_Children){
        itr.second.SetTranslation(Translation);
    }
}

const Model* Object::GetModel() const {
    return m_Model;
}

void Object::SetModel(Model* model) {
    m_Model = model;
}

const Vector& Object::GetScaling() const {
    return m_Scaling;
}

void Object::SetScaling(Vector Scaling) {
    m_Scaling = Scaling;
    for(auto itr:m_Children){
        itr.second.SetScaling(Scaling);
    }
}



void Object::addChildObject(Object& Object) {
    m_Children.insert(std::make_pair(Object.GetName(),Object));
}


void Object::draw() {
    if(m_Model != NULL){
        m_Model->draw(m_Translation, m_Scaling, m_RotationAxis, m_RotationAngle);
    }
    //Draw Children
    for(auto itr:m_Children){
        itr.second.draw();
    }
}

Object* Object::hasChild(std::string& name) {
    try{
       
        return &(m_Children.at(name));
    }catch(std::out_of_range& oor){
        return NULL;
    }
}

bool Object::adoption(std::string& parentName, Object& obj) {
    Object* parent = this->hasChild(parentName);
    
    if(parent != NULL){
        parent->addChildObject(obj);
        return true;
    }else{
        for(auto itr:m_Children){
            if(itr.second.adoption(parentName, obj)){   //Parent found somewhere
                return true;
            }
        }
        return false;
    }
}