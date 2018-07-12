#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "HitInformation.hpp"
#include <glm/ext.hpp>
#include <iostream>


class Primitive {
public:
  virtual ~Primitive();
  virtual void hitTest(const Ray& r, HitInformation& hit_info) = 0;
protected:
    bool kramer(const glm::vec4& origin,
                const glm::vec4& direction,
                const glm::vec3& p1,
                const glm::vec3& p2,
                const glm::vec3& p3,
                double& t);
    Primitive* m_primitive;
};

class Sphere : public Primitive {
public:
    Sphere();
    virtual ~Sphere();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);
};

class Cube : public Primitive {
public:
    Cube();
    virtual ~Cube();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);
};

class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::vec3& pos, double radius)
        : m_pos(pos), m_radius(radius)
    {
    }
    virtual ~NonhierSphere();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);

private:
    glm::vec3 m_pos;
    double m_radius;
};

class NonhierBox : public Primitive {
public:
    NonhierBox(const glm::vec3& pos, double size)
        : m_pos(pos), m_size(size)
    {
    }
  
    virtual ~NonhierBox();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);

private:
    glm::vec3 m_pos;
    double m_size;
};

class Cylinder : public Primitive {
public:
    Cylinder();
    virtual ~Cylinder();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);
    
private:
    glm::vec3 m_pos;
    double m_radius;
    double m_height;
};

class NonhierCylinder : public Primitive {
public:
    NonhierCylinder(const glm::vec3& pos, double radius, double height)
    : m_pos(pos), m_radius(radius), m_height(height)
    {
        std::cout << "created cylinder with: " << std::endl;
        std::cout << "position: " << glm::to_string(m_pos) << std::endl;
        std::cout << "radius: " << m_radius << std::endl;
        std::cout << "height: " << m_height << std::endl;

    }
    
    virtual ~NonhierCylinder();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);
    
private:
    glm::vec3 m_pos;
    double m_radius;
    double m_height;
};


class Cone : public Primitive {
public:
    Cone();
    virtual ~Cone();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);
    
private:
    glm::vec3 m_pos;
    double m_radius;
    double m_height;
};

class NonhierCone : public Primitive {
public:
    NonhierCone(const glm::vec3& pos, double radius, double height)
    : m_pos(pos), m_radius(radius), m_height(height)
    {
    }
    
    virtual ~NonhierCone();
    virtual void hitTest(const Ray& r, HitInformation& hit_info);
    
private:
    glm::vec3 m_pos;
    double m_radius;
    double m_height;
};
