//
//  HitInformation.cpp
//  A4
//
//  Created by Sakib Rabbany on 2018-06-17.
//  Copyright © 2018 none. All rights reserved.
//

#include "HitInformation.hpp"
#include <glm/glm.hpp>


HitInformation::HitInformation()
    :   hit(false),
        incident_ray(Ray(glm::vec4(0), glm::vec4(0))),
        t(0),
        hit_point(glm::vec4(0)),
        normal(glm::vec4(0)),
        node(nullptr),
        phong_mat(nullptr)
{
}

HitInformation::HitInformation(const Ray& r)
    :   hit(false),
        incident_ray(Ray(r)),
        t(0),
        hit_point(glm::vec4(0)),
        normal(glm::vec4(0)),
        node(nullptr),
        phong_mat(nullptr)
{
}

HitInformation::HitInformation(const Ray& r, double t)
    :   hit(false),
        incident_ray(Ray(r)),
        t(t),
        hit_point(glm::vec4(0)),
        normal(glm::vec4(0)),
        node(nullptr),
        phong_mat(nullptr)
{
}

HitInformation::HitInformation(bool intersect, const Ray& r, double t)
    :   hit(intersect),
        incident_ray(Ray(r)),
        t(t),
        hit_point(glm::vec4(0)),
        normal(glm::vec4(0)),
        node(nullptr),
        phong_mat(nullptr)
{
}

