/* 
 * File:   Object.h
 * Author: phil
 *
 * Created on 7. September 2015, 20:09
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include <string>
#include <vector>
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
    void SetRotationAxis(const Vector& RotationAxis);
    const Vector& GetTranslation() const;
    void SetTranslation(const Vector& Translation);
    const Model* GetModel() const;
    void SetModel(Model* model);
    const Vector& GetScaling() const;
    void SetScaling(const Vector& Scaling);
    
    void addChildObject(Object& Object);
    void draw();
    
protected:
    std::string m_Name;
    Model* m_Model = NULL;
    Vector m_Translation;
    Vector m_RotationAxis;
    Vector m_Scaling;
    GLfloat m_RotationAngle;
    std::vector<Object> m_Children;
};

#endif	/* OBJECT_H */

