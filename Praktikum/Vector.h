/* 
 * File:   Vector.h
 * Author: phil
 *
 * Created on 4. August 2015, 14:58
 */

#ifndef VECTOR_H
#define VECTOR_H

class Vector{
public:
    float X;
    float Y;
    float Z;
    
    Vector(float x, float y, float z);
    Vector();
    
    float dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector& operator-=(const Vector& v);
    Vector operator*(float c) const;
    Vector operator-() const;
    Vector& normalize();
    float length() const;
    float lengthSquared() const;
    Vector reflection(const Vector& normal) const;
    bool triangleIntersection(const Vector& d, const Vector& a, const Vector& b,
                                const Vector& c, float& s) const;
};
#endif	/* VECTOR_H */

