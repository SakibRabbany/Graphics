#include "PhongMaterial.hpp"


static PhongMaterial air =  PhongMaterial(glm::vec3(0), glm::vec3(0), 0.0, glm::vec4(0), 1.0);
static PhongMaterial water =  PhongMaterial(glm::vec3(0), glm::vec3(0), 0.0, glm::vec4(0), 1.33);
static PhongMaterial glass =  PhongMaterial(glm::vec3(0), glm::vec3(0), 0.0, glm::vec4(0), 1.52);

PhongMaterial* PhongMaterial::Air = &air;
PhongMaterial* PhongMaterial::Water = &water;
PhongMaterial* PhongMaterial::Glass = &glass;



PhongMaterial::PhongMaterial(
                             const glm::vec3& kd, const glm::vec3& ks, double shininess, const glm::vec4& glossy_coeff, double refractive_index)
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
    , m_glossy_coefficients(glossy_coeff)
    , m_refractive_index(refractive_index)
{}

PhongMaterial::~PhongMaterial()
{}

PhongMaterial& PhongMaterial::operator=(const PhongMaterial &other) {
    this->m_kd = other.m_kd;
    this->m_ks = other.m_ks;
    this->m_shininess = other.m_shininess;
    this->m_glossy_coefficients = other.m_glossy_coefficients;
    this->m_refractive_index = other.m_refractive_index;
    
    return *this;
}

PhongMaterial& PhongMaterial::operator=(PhongMaterial other) {
    this->m_kd = other.m_kd;
    this->m_ks = other.m_ks;
    this->m_shininess = other.m_shininess;
    this->m_glossy_coefficients = other.m_glossy_coefficients;
    this->m_refractive_index = other.m_refractive_index;
    
    return *this;
}
