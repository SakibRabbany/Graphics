//
//  Texture.cpp
//  A4
//
//  Created by Sakib Rabbany on 2018-07-12.
//  Copyright © 2018 none. All rights reserved.
//

#include "Texture.hpp"
#include <lodepng/lodepng.h>
#include <iostream>
#include <string>

Texture::Texture(){}

Texture::~Texture(){}

glm::vec3 Texture::getColor(double u, double v) {
    return glm::vec3(0);
}

RasterImage::RasterImage(std::string file_name) : Texture() {
    m_file_name = file_name;
    std::vector<unsigned char> buffer;
    
    lodepng::load_file(buffer, m_file_name);
    
    unsigned error = lodepng::decode(m_image, m_width, m_height, buffer);
    
    if(error){
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(-1);
    }
    std::cout << "image height: " << m_height << " image width: " << m_width << std::endl;
}

RasterImage::~RasterImage(){}

glm::vec3 RasterImage::getColor(double u, double v) {
    std::cout << "in raster image" << std::endl;
    double di, dj;
    int x, y;
    
    std::cout << "u: " << u << "v: " << v << std::endl;
    
    di = (m_width - 1) * u;
    dj = (m_height - 1) * v;
    
    x = (int)di;
    y = (int)dj;
    
    return retrieveColour(x, y)/255.0;
    
}

glm::vec3 RasterImage::retrieveColour(int x, int y) {
    int position = 4*(m_width * y + x);
    glm::vec3 pixel(0);
    pixel.r = m_image[position + 0];
    pixel.g = m_image[position + 1];
    pixel.b = m_image[position + 2];

    return pixel;
}


