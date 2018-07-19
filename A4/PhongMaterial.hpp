#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
    PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, const glm::vec4& glossy_coeff, double refractive_index);
  virtual ~PhongMaterial();
    
    glm::vec3 m_kd;
    glm::vec3 m_ks;
    
    double m_shininess;
    double m_refractive_index;
    glm::vec4 m_glossy_coefficients;
    
    static PhongMaterial* Air;
    static PhongMaterial* Water;
    static PhongMaterial* Glass;
    
    PhongMaterial& operator=(const PhongMaterial& other);
    PhongMaterial& operator=(PhongMaterial other);

private:
  
};
