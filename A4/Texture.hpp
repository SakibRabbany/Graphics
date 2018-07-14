//
//  Texture.hpp
//  A4
//
//  Created by Sakib Rabbany on 2018-07-12.
//  Copyright © 2018 none. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

class Texture {
public:
    Texture();
    virtual ~Texture();
    
    virtual glm::vec3 getColor(double u, double v);
};


class RasterImage : public Texture {
public:
    RasterImage(std::string file_name);
    virtual ~RasterImage();
    
    virtual glm::vec3 getColor(double u, double v);
    
private:
    std::vector <unsigned char> m_image;
    unsigned m_height, m_width;
    std::string m_file_name;
    unsigned num_component = 4;
    
    glm::vec3 retrieveColour(int x, int y);
};

#endif /* Texture_hpp */
