#pragma once

#include <iosfwd>

#include <glm/glm.hpp>

// Represents a simple point light.
struct Light {
  Light();
    Light(glm::vec3 colour, glm::vec3 position);
    
  virtual glm::vec3 getLightPosition() const;
  
  glm::vec3 colour;
  glm::vec3 position;
  double falloff[3];
  bool is_point_source;
};

struct AreaLight : Light {
    AreaLight();
    AreaLight(glm::vec3 colour, glm::vec3 position, double radius);
    
    virtual glm::vec3 getLightPosition() const;

    double radius;
};

std::ostream& operator<<(std::ostream& out, const Light& l);

