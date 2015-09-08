/* 
 * File:   Object.h
 * Author: phil
 *
 * Created on 7. September 2015, 20:09
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include <iostream>
#include <string>
#include <map>
#include "Model.h"
#include "Vector.h"

class Object {
public:
    Object();
    Object(std::string name, Vector translation, Vector scaling, Vector rotAxis, GLfloat rotAngle);
    std::string GetName() const;
    void SetName(std::string Name);
    GLfloat GetRotationAngle() const;
    void SetRotationAngle(GLfloat RotationAngle);
    const Vector& GetRotationAxis() const;
    void SetRotationAxis(Vector RotationAxis);
    const Vector& GetTranslation() const;
    void SetTranslation(Vector Translation);
    const Model* GetModel() const;
    void SetModel(Model* model);
    const Vector& GetScaling() const;
    void SetScaling(Vector Scaling);
    
    void addChildObject(Object& Object);
    bool adoption(std::string& parentName, Object& obj);
    Object* findObject(std::string& parentName);
    void draw();
    
protected:
    
    Object* hasChild(std::string& name);
    std::string m_Name;
    Model* m_Model = NULL;
    Vector m_Translation;
    Vector m_RotationAxis;
    Vector m_Scaling;
    GLfloat m_RotationAngle;
    std::map<std::string, Object> m_Children;
    
};

#endif	/* OBJECT_H */

