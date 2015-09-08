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
}

void Object::SetRotationAxis(const Vector& RotationAxis) {
    m_RotationAxis = RotationAxis;
}

void Object::SetTranslation(const Vector& Translation) {
    m_Translation = Translation;
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

void Object::SetScaling(const Vector& Scaling) {
    m_Scaling = Scaling;
}



void Object::addChildObject(Object& Object) {
    m_Children.push_back(Object);
}


void Object::draw() {
    if(m_Model != NULL){
        m_Model->draw(m_Translation, m_Scaling, m_RotationAxis, m_RotationAngle);
    }
    
    //Vllt. hier Kinder zeichnen
}


