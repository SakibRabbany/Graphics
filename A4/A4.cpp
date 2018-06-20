#include <glm/ext.hpp>
#include <limits.h>

#include "A4.hpp"
#include "Ray.hpp"
#include <random>


#define DISTANCE_T0_IMAGE_PLANE 10
#define MAX_BOUNCE 100
#define EPSILON 0.01
#define ANTIALIAS 0
#define REFLECTION 0

static SceneNode* m_root;
static int pixel_count = 0;
static glm::vec3 AmbientLight;

int rand_int(int min, int max) {
    std::random_device rd;
    std:: mt19937 rng(rd());
    std::uniform_int_distribution<int> uni (min, max);
    
    return uni(rng);
}

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;
    
    /////////
    m_root = root;
    AmbientLight = ambient;
    
    glm::vec4 origin, direction, direction1, direction2, direction3, direction4, direction5, direction6, direction7, direction8, direction9;
    glm::vec3 color, color1, color2, color3, color4, color5, color6, color7, color8, color9, col_sum;
    origin = glm::vec4(eye, 1);

    glm::mat4 transformation = getPixelToWorldTransform(eye, view, up, fovy, image.width(), image.height());
    
    glm::vec4 world_coord, world_coord1, world_coord2, world_coord3, world_coord4, world_coord5, world_coord6, world_coord7, world_coord8, world_coord9;
    
	size_t h = image.height();
	size_t w = image.width();

	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
//            // Red: increasing from top to bottom
//            image(x, y, 0) = (double)y / h;
//            // Green: increasing from left to right
//            image(x, y, 1) = (double)x / w;
//            // Blue: in lower-left and upper-right corners
//            image(x, y, 2) = ((y < h/2 && x < w/2)
//                          || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
            
            if (ANTIALIAS) {
                world_coord1 = pixelToWorld(glm::vec3(       x,               y,      0), transformation);
                world_coord2 = pixelToWorld(glm::vec3(x + 1.0f/3.0f,         y,0),       transformation);
                world_coord3 = pixelToWorld(glm::vec3(x + 2.0f/3.0f,         y,0),       transformation);
                world_coord4 = pixelToWorld(glm::vec3(        x,       y + 1.0f/3.0f,0), transformation);
                world_coord5 = pixelToWorld(glm::vec3(x + 1.0f/3.0f, y + 1.0f/3.0f,0), transformation);
                world_coord6 = pixelToWorld(glm::vec3(x + 2.0f/3.0f, y + 1.0f/3.0f,0), transformation);
                world_coord7 = pixelToWorld(glm::vec3(        x,       y + 2.0f/3.0f,0), transformation);
                world_coord8 = pixelToWorld(glm::vec3(x + 1.0f/3.0f, y + 2.0f/3.0f,0), transformation);
                world_coord9 = pixelToWorld(glm::vec3(x + 2.0f/3.0f, y + 2.0f/3.0f,0), transformation);

                direction1 = (world_coord1 - origin);
                direction2 = (world_coord2 - origin);
                direction3 = (world_coord3 - origin);
                direction4 = (world_coord4 - origin);
                direction5 = (world_coord5 - origin);
                direction6 = (world_coord6 - origin);
                direction7 = (world_coord7 - origin);
                direction8 = (world_coord8 - origin);
                direction9 = (world_coord9 - origin);

                
                Ray r1 = Ray(origin, direction1);
                Ray r2 = Ray(origin, direction2);
                Ray r3 = Ray(origin, direction3);
                Ray r4 = Ray(origin, direction4);
                Ray r5 = Ray(origin, direction5);
                Ray r6 = Ray(origin, direction6);
                Ray r7 = Ray(origin, direction7);
                Ray r8 = Ray(origin, direction8);
                Ray r9 = Ray(origin, direction9);

                color1 = rayColor(r1, lights, 0);
                color2 = rayColor(r2, lights, 0);
                color3 = rayColor(r3, lights, 0);
                color4 = rayColor(r4, lights, 0);
                color5 = rayColor(r5, lights, 0);
                color6 = rayColor(r6, lights, 0);
                color7 = rayColor(r7, lights, 0);
                color8 = rayColor(r8, lights, 0);
                color9 = rayColor(r9, lights, 0);
                
                col_sum = color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9;

                col_sum = col_sum/9;
                pixel_count++;
                
                double prog = (double)pixel_count/(double)(image.width()*image.height());
                
                std::cout << "Progress: " << prog*100 << std::endl;
                
                image(x, y, 0) = col_sum.r;
                image(x, y, 1) = col_sum.g;
                image(x, y, 2) = col_sum.b;
            } else {
                glm::vec4 world_coord = pixelToWorld(glm::vec3(x, y,0), transformation);
                direction = (world_coord - origin);
                
                Ray r = Ray(origin, direction);
                color = glm::vec3(0);
                
                pixel_count++;
                
                double prog = (double)pixel_count/(double)(image.width()*image.height());
                
                std::cout << "Progress: " << prog*100 << std::endl;
                
                Color rc = rayColor(r, lights, 0);
                image(x, y, 0) = rc.r;
                image(x, y, 1) = rc.g;
                image(x, y, 2) = rc.b;
            }
		}
	}

}


Color rayColor(const Ray& r, const std::list<Light *> & lights, int counter) {
    Color kd, ks , ke, col;
    col = glm::vec3(0);
    
    if (counter > MAX_BOUNCE) return col;
    
    HitInformation hit_info = HitInformation(r);
    
    hit(r, m_root, hit_info);
    
    if (hit_info.hit) {
        col += hit_info.phong_mat->m_kd * AmbientLight;
        
        if (hit_info.phong_mat->m_kd != glm::vec3(0)) {
            col += directLight(lights, hit_info, counter);
        }

        
        if (REFLECTION) {
            if (hit_info.phong_mat->m_ks != glm::vec3(0) and counter < MAX_BOUNCE){
                counter++;
                glm::vec4 reflected_direction = hit_info.incident_ray.direction - (2 * glm::dot(hit_info.incident_ray.direction, hit_info.normal) * hit_info.normal);
                glm::vec4 intersection_point = hit_info.incident_ray.origin + (hit_info.t * hit_info.incident_ray.direction);
                
                Ray reflected_ray = Ray(intersection_point, reflected_direction);
                
                col += hit_info.phong_mat->m_ks * rayColor(reflected_ray, lights, counter) * hit_info.phong_mat->m_shininess/100;
            }
        }
    } else {
        int number = rand_int(1, 100);
        if (number == 1)
            col = glm::vec3(1,1,1);
        else
            col = glm::vec3(0,0,0);
    }
    
    return col;
    
}


Color directLight(const std::list<Light *>& lights, HitInformation& hit_info, int counter) {
    Color col = glm::vec3(0);
    
    if (counter > MAX_BOUNCE) {
        return col;
    }
    
    glm::vec4       intersection_point,
                    light_position,
                    shadow_ray_origin,
                    shadow_ray_direction,
                    intersection_normal,
                    reflected_ray_direction,
                    view_direction;
    
    double distance_to_light, distance_to_hit;
    
    intersection_point = hit_info.incident_ray.pointAtParameterT(hit_info.t);
//    intersection_point = hit_info.hit_point;
    intersection_normal = (hit_info.normal);
    glm::vec3 kd, ks;
    
    kd = hit_info.phong_mat->m_kd;
    ks = hit_info.phong_mat->m_ks;
    
    
    for (const Light* light : lights) {
        light_position = glm::vec4(light->position, 1);
        
        shadow_ray_direction = glm::normalize(light_position - intersection_point);
        shadow_ray_origin = intersection_point + (shadow_ray_direction *EPSILON);
        
//        std::cout << "initial shadow ray: " << glm::to_string(shadow_ray_origin) << " " << glm::to_string(shadow_ray_direction) << std::endl;

        
        Ray shadow_ray = Ray(shadow_ray_origin, shadow_ray_direction);
        
        distance_to_light = glm::length(light_position - shadow_ray_origin);
        
        HitInformation shadow_ray_info = HitInformation(shadow_ray);
        hit(shadow_ray, m_root, shadow_ray_info);
        
//        std::cout << "final shadow ray: " << glm::to_string(shadow_ray_origin) << " " << glm::to_string(shadow_ray_direction) << std::endl;

        
        if (shadow_ray_info.hit) {
            distance_to_hit = glm::length((shadow_ray.pointAtParameterT(shadow_ray_info.t) - shadow_ray_origin));
            if (distance_to_hit < distance_to_light) continue;
        }
        
        shadow_ray_direction = glm::normalize(shadow_ray_direction);
        intersection_normal = glm::normalize(intersection_normal);
        
        
        auto l_dot_n = glm::dot(shadow_ray_direction, intersection_normal) < 0 ? 0.0 : glm::dot(shadow_ray_direction, intersection_normal);
//        auto l_dot_n = glm::dot(shadow_ray_direction, intersection_normal);

        if (kd != glm::vec3(0)){
            col += kd * l_dot_n * light->colour;
        }
       
        if(ks != glm::vec3(0)) {
            reflected_ray_direction = (2 * l_dot_n * intersection_normal) - shadow_ray_direction;
            view_direction = glm::normalize(-hit_info.incident_ray.direction);
            auto r_dot_v = glm::dot(reflected_ray_direction, view_direction);
            col += ks * pow(r_dot_v, hit_info.phong_mat->m_shininess) * light->colour;
        }
        
    }
    
    return col;
}


glm::mat4 getPixelToWorldTransform(const glm::vec3 & eye,
                                   const glm::vec3 & view,
                                   const glm::vec3 & up,
                                   double fovy,
                                   double image_width,
                                   double image_height) {
    //double distance = 5;
    double plane_width, plane_height;
    plane_height = 2 * DISTANCE_T0_IMAGE_PLANE * glm::tan(glm::radians(fovy/2));
    plane_width = (image_width/image_height) * plane_height;
    
    glm::mat4 t1, s2, r3, t4;
    
    t1 = T1(image_width, image_height, DISTANCE_T0_IMAGE_PLANE);
    s2 = S2(image_width, image_height, plane_width, plane_height);
    r3 = R3(eye, view, up);
    t4 = T4(eye);
    
    return t4 * r3 * s2 * t1;
}


glm::vec4 pixelToWorld(glm::vec3 px_pos, glm::mat4& transformation){
//    std::cout << px_pos.x<< " " << px_pos.y << std::endl;
    return transformation * glm::vec4(px_pos, 1);
}


glm::mat4 T1(double image_width, double image_height, double distance) {
    return glm::translate(glm::mat4(1), glm::vec3(-(image_width/2), -(image_height/2), distance));
}

glm::mat4 S2(double image_width, double image_height, double plane_width, double plane_height) {
    return glm::scale(glm::mat4(1), glm::vec3(-(plane_width/image_width), plane_height/image_height, 1));
}

glm::mat4 R3(const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up){
    
    glm::vec3 w = glm::normalize(glm::vec3(view - eye));
    glm::vec3 u = -glm::normalize(glm::cross(w, up));
    glm::vec3 v = glm::cross(u, w);
    
    glm::vec4 col0 = glm::vec4(u.x, u.y, u.z, 0);
    glm::vec4 col1 = glm::vec4(v.x, v.y, v.z, 0);
    glm::vec4 col2 = glm::vec4(w.x, w.y, w.z, 0);
    glm::vec4 col3 = glm::vec4(  0,   0,   0, 1);

    return glm::mat4(col0, col1, col2, col3);
}

glm::mat4 T4(const glm::vec3 & eye) {
    return glm::translate(glm::mat4(1), eye);
}

void hit (const Ray& r, SceneNode* root, HitInformation& hit_info) {
    root->hitTest(r, hit_info);
}




