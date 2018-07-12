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
        double m_min;
//        if (m_min > 0 and m_min < min_t) {
//            min_t = m_min;
//            hit = true;
//        }
        if (roots[0] < roots[1]) {
            if (roots[0] > 0 and roots[0] < min_t) {
                min_t = roots[0];
                hit = true;
            }
        } else {
            if (roots[1] > 0 and roots[1] < min_t) {
                min_t = roots[1];
                hit = true;
            }
        }
    }
    
    if (hit) {
        hit_info.t = min_t;
        hit_info.hit = hit;
        hit_info.normal = glm::normalize(((r.origin + min_t * r.direction) - glm::vec4(m_pos,1)));
        hit_info.hit_point = r.origin + ((min_t - 0.01) * r.direction);
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
        hit_info.hit_point = r.origin + ((min_t - 0.01) * r.direction);
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

Cylinder::Cylinder(){
    m_primitive = new NonhierCylinder(glm::vec3(0), 1, 1);
}

void Cylinder::hitTest(const Ray &r, HitInformation &hit_info) {
    m_primitive->hitTest(r, hit_info);
}

Cylinder::~Cylinder(){
    delete m_primitive;
}

NonhierCylinder::~NonhierCylinder()
{
}

void NonhierCylinder::hitTest(const Ray &r, HitInformation &hit_info) {
    double a, b, c;
    bool hit = false;
    double min_t = DBL_MAX;
    glm::vec4 normal;
    glm::vec4 hit_point;
    
    a = (r.direction.x * r.direction.x) + (r.direction.z * r.direction.z);
    b = (2 * r.origin.x * r.direction.x) + (2 * r.origin.z * r.direction.z);
    c = (r.origin.x * r.origin.x) + (r.origin.z * r.origin.z) - (m_radius * m_radius);
    
    double roots[2];
    size_t num_roots = quadraticRoots(a, b, c, roots);
    
    double t1, t2, y1, y2, y_min, y_max;
    
//    y_min = m_pos.y;
//    y_max = m_pos.y + m_height;
    y_min = -1;
    y_max = 1;
    
    if (num_roots == 0) {
        return;
    } else if (num_roots == 1 and roots[0] > 0) {
        t1 = roots[0];
        y1 = r.origin.y + t1 * r.direction.y;
        if (y1 >= y_min and y1 <= y_max) {
            min_t = t1;
            hit = true;
            hit_point = r.origin + min_t * r.direction;
            normal = glm::normalize(glm::vec4(hit_point.x - m_pos.x, 0 , hit_point.z - m_pos.z, 0));
        }
        
    } else if (num_roots == 2) {
        if (roots[0] > roots[1]) {
            t1 = roots[1];
            t2 = roots[0];
        } else {
            t1 = roots[0];
            t2 = roots[1];
        }
        y1 = r.origin.y + t1 * r.direction.y;
        y2 = r.origin.y + t2 * r.direction.y;
        
        if (y1 < y_min){
            if (y2 < y_min) hit = false;
            else {
                // hit the cap
                float th = t1 + (t2 - t1) * (y1 + y_max) / (y1 - y2);
                if (th <= 0) return;
                min_t = th;
                hit = true;

                hit_point = r.origin + (r.direction * th);
                normal = glm::vec4(0, -1, 0, 0);
            }
        } else if (y1 >= y_min && y1 <= y_max) {
            // hit the cylinder bit
            if (t1 <= 0) return;
            hit = true;
            min_t = t1;
            
            hit_point = r.origin + (r.direction * t1);
            normal = glm::normalize(glm::vec4(hit_point.x - m_pos.x, 0 , hit_point.z - m_pos.z, 0));
        } else if (y1 > y_max) {
            if (y2 > y_max) return;
            else {
                // hit the cap
                float th = t1 + (t2 - t1) * (y1 + y_min) / (y1 - y2);
                if (th <= 0) return;
                
                min_t = th;
                hit = true;
                
                hit_point = r.origin + (r.direction * th);
                normal = glm::vec4(0, 1, 0, 0);
            }
        }
    }
    
    if (hit) {
        hit_info.t = min_t - 0.00001;
        hit_info.hit = true;
        hit_info.normal = normal;
        hit_info.hit_point = hit_point;
    }
}

Cone::Cone(){
    m_primitive = new NonhierCone(glm::vec3(0), 1, 1);
}

void Cone::hitTest(const Ray &r, HitInformation &hit_info){
    m_primitive->hitTest(r, hit_info);
}

Cone::~Cone(){
    delete m_primitive;
}

NonhierCone::~NonhierCone()
{
}

void NonhierCone::hitTest(const Ray &r, HitInformation &hit_info){
    
//    glm::vec3 axis = glm::vec3(0,1,0);
//    glm::vec3 theta = (axis/glm::length(axis));
//    float m = pow(m_radius,2)/pow(glm::length(axis),2);
//    glm::vec3 w = (r.origin - (m_pos + m_height));
//
//    float a = glm::dot(ad_,ad_) - m*(pow(glm::dot(ad_,theta),2)) - pow(glm::dot(ad_,theta),2);
//    float b = 2.f*( glm::dot(ad_,w) - m*glm::dot(ad_,theta)*glm::dot(w,theta) - glm::dot(ad_,theta)*glm::dot(w,theta) );
//    float c = glm::dot(w,w) - m*pow(glm::dot(w,theta),2) - pow(glm::dot(w,theta),2);
//
//    float Discriminant = pow(b,2) - (4.f*a*c);
//
//    if (Discriminant >= 0)
//        return std::array<glm::vec3,2>{{
//            (ap_+static_cast<float>(((-b) - sqrt(Discriminant))/(2.f*a))*ad_),
//            (ap_+static_cast<float>(((-b) + sqrt(Discriminant))/(2.f*a))*ad_)
//        }};
    
 
    
//    double t_near = DOUBLE_MAX, t_far = 0;
//    double t[2];
//
//    double x_D = ray.D[0];
//    double y_D = ray.D[1];
//    double z_D = ray.D[2];
//    double x_O = ray.O[0];
//    double y_O = ray.O[1];
//    double z_O = ray.O[2];
//
    
    
    
//    double fac = (m_radius * m_radius) / (m_height * m_height);
//
//    // cylinder part
//
//    double yA = (fac) * r.direction.y * r.direction.y;
//    double yB = (2 * fac * r.origin.y * r.direction.y) - (2 * (m_radius * m_radius) / m_height) * r.direction.y;
//    double yC = (fac * r.origin.y * r.origin.y) - ((2 * (m_radius * m_radius) / m_height) * r.origin.y) + (m_radius * m_radius);
//
//    // TODO clean up
////    double A = (x_D * x_D) + (z_D * z_D) - yA;
////    double B = (2 * x_O * x_D) + (2 * z_O * z_D) - yB;
////    double C = (x_O * x_O) + (z_O * z_O) - yC;
//
//    double a, b, c;
//    a = (r.direction.x * r.direction.x) + (r.direction.z * r.direction.z) - yA;
//    b = (2 * r.origin.x * r.direction.x) + (2 * r.origin.z * r.direction.z) - yB;
//    c = (r.origin.x * r.origin.x) + (r.origin.z * r.origin.z) - yC;
    
    
    
    
//    double d = (B * B) - (4 * A * C);
//
//    if (d >= 0) {
//        t[0] = (-B - sqrt(d)) / (2 * A);
//        t[1] = (-B + sqrt(d)) / (2 * A);
//        for (int i = 0; i < 2; i++) {
////            if (t[i] > EPSILON) { // So it doesn't cast shadows on itself
//                // find intersection
//                Intersection temp;
//                temp.NEAR = ray.O + t[i] * ray.D;
//                if (temp.NEAR[1] <= HEIGHT && temp.NEAR[1] >= 0) { // valid intersection point
//
//                    double a = 1, d = (-1 * RADIUS * RADIUS) / (double) (HEIGHT * HEIGHT), f = 1, h = (2
//                                                                                                       * RADIUS * RADIUS) / HEIGHT;
//
//                    double B = std::sqrt(2);
//
//                    double mag = std::sqrt(temp.NEAR[0] * temp.NEAR[0] + temp.NEAR[2] * temp.NEAR[2]);
//
//                    double costheta = temp.NEAR[0] / (double) (mag);
//
//                    temp.NORMAL[0] = 1 / B * costheta;
//                    temp.NORMAL[1] = 1 / B;
//                    temp.NORMAL[2] = 1 / B * (1 - costheta);
//
//                    if (t[i] > t_far) {
//                        t_far = t[i];
//                        intersection.FAR = temp.NEAR;
//                        intersection.FAR_NORMAL = temp.NORMAL;
//                    }
//                    if (t[i] < t_near) {
//                        t_near = t[i];
//                        intersection.NEAR = temp.NEAR;
//                        intersection.NORMAL = temp.NORMAL;
//                    }
//                }
//
//        }
//    }
//
//    intersection.NORMAL.normalize();
//
//    // base
//    Intersection temp;
//    double t_cap;
//    bool hit = intersect_circle(ray, CAP_CENTRE, RADIUS, CAP_NORMAL, temp, t_cap);
//
//    if (hit) {
//        if (t_cap > t_far) {
//            t_far = t_cap;
//            intersection.FAR = temp.NEAR;
//            intersection.FAR_NORMAL = temp.NORMAL;
//        }
//        if (t_cap < t_near) {
//            t_near = t_cap;
//            intersection.NEAR = temp.NEAR;
//            intersection.NORMAL = temp.NORMAL;
//        }
//    }
//
//    if (t_near < DOUBLE_MAX) {
//        return true;
//    } else {
//        return false;
//    }
//
//    /////
    double a, b, c;
    a = (r.direction.x * r.direction.x) + (r.direction.z * r.direction.z) - (r.direction.y * r.direction.y);
    b = (2 * r.origin.x * r.direction.x) + (2 * r.origin.z * r.direction.z) - (2 * r.origin.y * r.direction.y);
    c = (r.origin.x * r.origin.x) + (r.origin.z * r.origin.z) - (r.origin.y * r.origin.y);

    double t1, t2, y1, y2, y_min, y_max;
    glm::vec4 hit_point;
    glm::vec4 normal;
    double min_t = DBL_MAX;
    bool hit = false;

    y_min = 0;
    y_max = 1;


    double roots[2];
    size_t num_roots = quadraticRoots(a, b, c, roots);

    if (num_roots == 0) {
        return;
    } else if (num_roots == 1 and roots[0] > 0) {
        t1 = roots[0];
        y1 = r.origin.y + t1 * r.direction.y;
        if (y1 >= y_min and y1 <= y_max) {
            min_t = t1;
            hit = true;
            
            hit_point = r.origin + t1 * r.direction;
            glm::vec3 v;
            v.x = hit_point.x - m_pos.x;
            v.y = 0;
            v.z = hit_point.z - m_pos.z;
            double m = sqrt(v.x * v.x + v.z * v.z);
            v.x /= m;
            v.z /= m;
            normal.x = v.x * m_height / m_radius;
            normal.y = m_radius / m_height;
            normal.z = v.z * m_height / m_radius;
            normal.w = 0;
            normal.y -= normal.y;
            normal = glm::normalize(normal);
        } else return;
    } else if (num_roots == 2) {
        if (roots[0] > roots[1]) {
            t1 = roots[1];
            t2 = roots[0];
        } else {
            t1 = roots[0];
            t2 = roots[1];
        }
        y1 = r.origin.y + t1 * r.direction.y;
        y2 = r.origin.y + t2 * r.direction.y;
        
        if (y1 < y_min) {
            if (y2 < y_min or y2 > y_max){
                return;
            } else {
                
                if (t2 <= 0) return;
                
                min_t = t2;
                hit = true;
                
                hit_point = r.origin + t2 * r.direction;
                glm::vec3 v;
                v.x = hit_point.x - m_pos.x;
                v.y = 0;
                v.z = hit_point.z - m_pos.z;
                double m = sqrt(v.x * v.x + v.z * v.z);
                v.x /= m;
                v.z /= m;
                normal.x = v.x * m_height / m_radius;
                normal.y = m_radius / m_height;
                normal.z = v.z * m_height / m_radius;
                normal.w = 0;
                normal.y -= normal.y;
                normal = glm::normalize(normal);
                
                
            }
        } else if (y1 > y_max) {
            if (y2 > y_max or y2 < y_min) {
                return;
            } else {
                
                double th = (y_max - r.origin.y) / r.direction.y; ////error
                hit_point = r.origin + th * r.direction;
                
                if (th <= 0) return;
                
                if (abs(hit_point.x) <= m_radius and abs(hit_point.z) <= m_radius){
                    min_t = th;
                    hit = true;
                    normal = glm::vec4(0, 1, 0, 0);
                }
            }
        } else {
            if (y2 < y_min) {
                return;
            } else {
                
                if (t1 <= 0) return;
                
                min_t = t1;
                hit = true;
                
                hit_point = r.origin + t1 * r.direction;
                glm::vec3 v;
                v.x = hit_point.x - m_pos.x;
                v.y = 0;
                v.z = hit_point.z - m_pos.z;
                double m = sqrt(v.x * v.x + v.z * v.z);
                v.x /= m;
                v.z /= m;
                normal.x = v.x * m_height / m_radius;
                normal.y = m_radius / m_height;
                normal.z = v.z * m_height / m_radius;
                normal.w = 0;
                normal.y -= normal.y;

                normal = glm::normalize(normal);
            }
        }
    }
    
    if (hit) {
        hit_info.hit = true;
        hit_info.t = min_t - 0.0000;
        hit_info.hit_point = hit_point;
        hit_info.normal = normal;
    }

}
