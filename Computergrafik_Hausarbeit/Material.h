//
//  Material.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 27.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Material__
#define __RealtimeRending__Material__

#include <iostream>
#include "Vector.h"
#include "Color.h"
#include "Texture.h"

class Material
{
public:
    Material( Vector kd, Vector ks, Vector ka, float ns, std::string name );
    Material();
    ~Material();
    const Color& getDiffuseColor() const;
    const Color& getSpecularColor() const;
    const Color& getAmbientColor() const;
    float getSpecularExponent() const;
    const Texture& getDiffuseTexture() const;
    const Texture& getSpecularTexture() const;
    const std::string& getName() const;
    
    void setDiffuseColor( const Color& color);
    void setSpecularColor( const Color& color);
    void setAmbientColor( const Color& color);
    void setSpecularExponent( const float exp);
    void setName( const std::string& name);
    bool setDiffuseTexture( const char* Filename);
    void setSpecularTexture( const char* Filename);
    void applyDiffuseTexture() const;

protected:
    Color m_DiffuseColor;
    Color m_SpecularColor;
    Color m_AmbientColor;
    Texture m_DiffuseTexture;
    Texture m_SpecularTexture;
    float m_SpecularExp;
    std::string m_Name;
    
};

#endif /* defined(__RealtimeRending__Material__) */
