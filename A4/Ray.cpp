//
//  Ray.cpp
//  A4
//
//  Created by Sakib Rabbany on 2018-06-16.
//  Copyright © 2018 none. All rights reserved.
//


#include "Ray.hpp"

Ray::Ray(glm::vec4 origin, glm::vec4 direction)
    : origin(origin),
      direction(direction)
{
    
}

Ray::Ray(const Ray& ray)
    : origin(ray.origin),
      direction(ray.direction)
{
    
}

glm::vec4 Ray::pointAtParameterT(double &t) {
    return origin + ((float)t * direction);
}
