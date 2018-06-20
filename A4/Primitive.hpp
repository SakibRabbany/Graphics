#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "HitInformation.hpp"
#include <glm/ext.hpp>


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
