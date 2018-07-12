#include <iostream>

#include <glm/ext.hpp>
#include <random>
#include "Light.hpp"

static std::default_random_engine generator;


Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0),
    is_point_source(true)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

Light::Light(glm::vec3 colour, glm::vec3 position)
: colour(colour),
  position(position),
  is_point_source(true)
{
    falloff[0] = 1.0;
    falloff[1] = 0.0;
    falloff[2] = 0.0;
}

glm::vec3 Light::getLightPosition() const {
    return position;
}

AreaLight::AreaLight()
: Light(),
  radius(0)
{
    is_point_source = false;
}

AreaLight::AreaLight(glm::vec3 colour, glm::vec3 position, double radius)
: Light(colour, position),
  radius(radius)
{
    is_point_source = false;
}

glm::vec3 AreaLight::getLightPosition() const {
    std::uniform_real_distribution<float> distribution(-radius, radius);
    
    float rand_x = distribution(generator);
    float rand_z = distribution(generator);
    
    return glm::vec3(rand_x, 0, rand_z) + position;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour) 
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
//    if (!l.is_point_source) {
//        const AreaLight al = l;
//        out << "radius: " << al.radius;
//    }
    out << "]";

  return out;
}
