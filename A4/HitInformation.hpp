//
//  HitInformation.hpp
//  A4
//
//  Created by Sakib Rabbany on 2018-06-17.
//  Copyright © 2018 none. All rights reserved.
//
#pragma once
#include "Ray.hpp"
#include "Texture.hpp"
#include "PhongMaterial.hpp"

class SceneNode;


struct HitInformation {
    bool hit;
    Ray incident_ray;
    double t;
    glm::vec4 hit_point;
    glm::vec4 normal;
    SceneNode* node;
    PhongMaterial* phong_mat;
    
    HitInformation();
    HitInformation(const Ray& r);
    HitInformation(const Ray& r, double t);
    HitInformation(bool intersect, const Ray& r, double t);
    
    Texture* getTexture();
    glm::vec2 getUV();
    double u, v;
};

