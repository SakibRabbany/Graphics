#include "Primitive.hpp"
#include "polyroots.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <glm/ext.hpp>
#include "Ray.hpp"

//void Primitive::hitTest(const Ray &r, HitInformation& hit_info){
//
//}

Primitive::~Primitive()
{
}

Sphere::Sphere() {
    m_primitive = new NonhierSphere(glm::vec3(0), 1);
}

void Sphere::hitTest(const Ray &r, HitInformation& hit_info) {
    m_primitive->hitTest(r, hit_info);
}

Sphere::~Sphere()
{
    delete m_primitive;
}

Cube::Cube(){
    m_primitive = new NonhierBox(glm::vec3(0), 1);
}

void Cube::hitTest(const Ray &r, HitInformation& hit_info) {
    m_primitive->hitTest(r, hit_info);
}

Cube::~Cube()
{
    delete m_primitive;
}

NonhierSphere::~NonhierSphere()
{
}

void NonhierSphere::hitTest(const Ray &r, HitInformation& hit_info) {
    
//    std::cout << "hittesting prim nonhiersphere" << std::endl;

    
    double a, b, c = 0;
    double min_t = DBL_MAX;
    bool hit = false;
    
    
    glm::vec4 a_sub_c = r.origin - glm::vec4(m_pos, 1);
    
    a = glm::dot(r.direction, r.direction);
    b = glm::dot(r.direction, a_sub_c) * 2;
    c = glm::dot(a_sub_c, a_sub_c) - (m_radius * m_radius);
    
    double roots[2];
    size_t num_roots = quadraticRoots(a, b, c, roots);
    
    if (num_roots == 1) {
        if (roots[0] > 0 and roots[0] < min_t) {
            min_t = roots[0];
            hit = true;
        }
    }
    
    if (num_roots == 2) {
        double m_min = std::min(roots[0], roots[1]);
        if (m_min > 0 and m_min < min_t) {
            min_t = m_min;
            hit = true;
        }
    }
    
    if (hit) {
        hit_info.t = min_t;
        hit_info.hit = hit;
        hit_info.normal = glm::normalize(((r.origin + min_t * r.direction) - glm::vec4(m_pos,1)));
//        if ((r.origin - glm::vec4(m_pos,1)).length() < m_radius) {
//            hit_info.normal = -hit_info.normal;
//        }
    }
}

void NonhierBox::hitTest(const Ray &r, HitInformation& hit_info) {

    std::vector<glm::vec3> verts = {
        glm::vec3(     0,      0,      0) + m_pos,
        glm::vec3(     0,      0, m_size) + m_pos,
        glm::vec3(     0, m_size,      0) + m_pos,
        glm::vec3(     0, m_size, m_size) + m_pos,
        glm::vec3(m_size,      0,      0) + m_pos,
        glm::vec3(m_size,      0, m_size) + m_pos,
        glm::vec3(m_size, m_size,      0) + m_pos,
        glm::vec3(m_size, m_size, m_size) + m_pos
    };
    
    std::vector<glm::vec3> faces = {
        glm::vec3(7, 3, 1),     // front
        glm::vec3(1, 5, 7),
        
        glm::vec3(4, 6, 7),     // left
        glm::vec3(7, 5, 4),
        
        glm::vec3(2, 6, 4),     // back
        glm::vec3(4, 0, 2),
        
        glm::vec3(3, 2, 0),     // right
        glm::vec3(0, 1, 3),
        
        glm::vec3(2, 3, 7),     // top
        glm::vec3(7, 6, 2),
        
        glm::vec3(0, 4, 5),     // bottom
        glm::vec3(5, 1, 0),
    };
    
    double t = 0;
    glm::vec4 normal;
    bool hit = false;
    
    double min_t = DBL_MAX;
    
    for (const glm::vec3& face : faces) {
        bool res = kramer(r.origin, r.direction, verts[(int)face[0]],
                                      verts[(int)face[1]],
                                      verts[(int)face[2]], t);
        if (res and t > 0 and t < min_t) {
            hit = true;
            normal = glm::vec4(glm::triangleNormal(verts[(int)face[0]],
                                                   verts[(int)face[1]],
                                                   verts[(int)face[2]]),0);
            min_t = t;
        }
    }
    
    if (hit) {
        hit_info.t = min_t;
        hit_info.hit = true;
        hit_info.normal = glm::normalize(normal);
    }
}

NonhierBox::~NonhierBox()
{
}

bool Primitive::kramer(const glm::vec4& origin,
                       const glm::vec4& direction,
                       const glm::vec3& p0,
                       const glm::vec3& p1,
                       const glm::vec3& p2,
                       double &t) {
    bool res(false);
    glm::mat3  mat, mat1, mat2, mat3;
    glm::vec3 o, dir, c1, c2, c3, r;
    float d, d1, d2, d3, gamma, beta;
    
    o = glm::vec3(origin.x, origin.y, origin.z);
    dir = glm::vec3(direction.x, direction.y, direction.z);
    r = o - p0;
    c1 = p1 - p0;
    c2 = p2 - p0;
    c3 = -dir;
    
    mat = glm::mat3(c1, c2, c3);
    d = glm::determinant(mat);
    
    mat1 = glm::mat3( r, c2, c3);
    mat2 = glm::mat3(c1,  r, c3);
    mat3 = glm::mat3(c1, c2,  r);
    
    d1 = glm::determinant(mat1);
    d2 = glm::determinant(mat2);
    d3 = glm::determinant(mat3);
    
    beta = d1/d;
    gamma = d2/d;
    t = d3/d;
    
    if (beta > 0 and gamma > 0 and (beta + gamma <= 1)) res = true;
    return res;
}
