//
//  Ray.hpp
//  A4
//
//  Created by Sakib Rabbany on 2018-06-16.
//  Copyright © 2018 none. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

class Ray {
public:
    glm::vec4 origin;
    glm::vec4 direction;

    Ray(glm::vec4 origin, glm::vec4 direction);
    Ray(const Ray& ray);
    
    glm::vec4 pointAtParameterT(double& t);
};
