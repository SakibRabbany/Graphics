#include <glm/ext.hpp>
#include <limits.h>

#include "A4.hpp"
#include "Ray.hpp"
#include <random>
#include <omp.h>


#define DISTANCE_T0_IMAGE_PLANE 10
#define MAX_BOUNCE 5
#define EPSILON 0.01
#define ANTIALIAS 0
#define REFLECTION 1
#define GLOSSY_REFLECTION 1
#define REFRACTION 1
#define GLOSSY_REFRACTION 1

#define NUM_SHADOW_RAY 15
#define NUM_GLOSSY_REFLECTION_RAY 20
#define NUM_GLOSSY_REFRACTION_RAY 20


static SceneNode* m_root;
static int pixel_count = 0;
static glm::vec3 AmbientLight;

static std::default_random_engine generator;


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
    updateSceneGraph();
    
    AmbientLight = ambient;
    
    glm::vec4 origin, direction, direction1, direction2, direction3, direction4, direction5, direction6, direction7, direction8, direction9;
    glm::vec3 color, color1, color2, color3, color4, color5, color6, color7, color8, color9, col_sum;
    origin = glm::vec4(eye, 1);

    glm::mat4 transformation = getPixelToWorldTransform(eye, view, up, fovy, image.width(), image.height());
    
    glm::vec4 world_coord, world_coord1, world_coord2, world_coord3, world_coord4, world_coord5, world_coord6, world_coord7, world_coord8, world_coord9;
    
	size_t h = image.height();
	size_t w = image.width();
    
    #pragma omp parallel for schedule(dynamic, 1) private(col_sum)

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

                color1 = rayColor(r1, lights, 0, PhongMaterial::Air, nullptr);
                color2 = rayColor(r2, lights, 0, PhongMaterial::Air, nullptr);
                color3 = rayColor(r3, lights, 0, PhongMaterial::Air, nullptr);
                color4 = rayColor(r4, lights, 0, PhongMaterial::Air, nullptr);
                color5 = rayColor(r5, lights, 0, PhongMaterial::Air, nullptr);
                color6 = rayColor(r6, lights, 0, PhongMaterial::Air, nullptr);
                color7 = rayColor(r7, lights, 0, PhongMaterial::Air, nullptr);
                color8 = rayColor(r8, lights, 0, PhongMaterial::Air, nullptr);
                color9 = rayColor(r9, lights, 0, PhongMaterial::Air, nullptr);
                
                col_sum = color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9;

                col_sum = col_sum/9;
                pixel_count++;
                
                double prog = (double)pixel_count/(double)(image.width()*image.height());
                
                std::cout << "Progress: " << prog*100 << std::endl;
                
                image(x, y, 0) = col_sum.r;
                image(x, y, 1) = col_sum.g;
                image(x, y, 2) = col_sum.b;
            } else {
                glm::vec4 world_coord = pixelToWorld(glm::vec3(x, y, 0), transformation);
                direction = (world_coord - origin);
                
                Ray r = Ray(origin, direction);
                color = glm::vec3(0);
                
                pixel_count++;
                
                double prog = (double)pixel_count/(double)(image.width()*image.height());
                
                std::cout << "Progress: " << prog*100 << std::endl;
                
                Color rc = rayColor(r, lights, 0, PhongMaterial::Air, nullptr);
                image(x, y, 0) = rc.r;
                image(x, y, 1) = rc.g;
                image(x, y, 2) = rc.b;
            }
		}
	}
    std::cout << "done" << std::endl;

}


Color rayColor(const Ray& r, const std::list<Light *> & lights, int counter, PhongMaterial* from_material, SceneNode* from_node) {
    Color kd, ks , ke, col;
    double reflectance;
    col = glm::vec3(0);
    
    if (counter > MAX_BOUNCE) return col;
    
    
    HitInformation hit_info = HitInformation(r);
    hit_info.from_material = from_material;
    
    hit(r, m_root, hit_info);
    PhongMaterial* temp_phong;
    
    if (hit_info.hit) {
        
        // to handle refraction
        if (from_node == hit_info.node) {
//            std::cout << "from node: " << from_node->m_name <<std::endl;
//            std::cout << "to node: " << hit_info.node->m_name <<std::endl;
//
//            std::cout << "hit the same node" << std::endl;
            hit_info.normal -= hit_info.normal;
            temp_phong = hit_info.phong_mat;
            hit_info.phong_mat = PhongMaterial::Air;
            from_node = nullptr;
        }
        
        col += hit_info.phong_mat->m_kd * AmbientLight;
        
        if (hit_info.phong_mat->m_kd != glm::vec3(0)) {
            col += directLight(lights, hit_info, counter);
        }
        
       
        if (hit_info.phong_mat->m_ks != glm::vec3(0) and counter < MAX_BOUNCE){
            
            reflectance = calculateReflectance(glm::normalize(glm::vec3(hit_info.normal)),
                                               glm::normalize(glm::vec3(hit_info.incident_ray.direction)),
                                               hit_info.from_material->m_refractive_index,
                                               hit_info.phong_mat->m_refractive_index);
            
            //        glm::vec4 reflected_direction = hit_info.incident_ray.direction - (2 * glm::dot(hit_info.incident_ray.direction, hit_info.normal) * hit_info.normal);
            glm::vec4 reflected_direction = getReflectedDirection(hit_info.incident_ray.direction, hit_info.normal);
            glm::vec4 intersection_point = hit_info.incident_ray.origin + ((hit_info.t - 0.001) * hit_info.incident_ray.direction);
            //                glm::vec4 intersection_point = hit_info.hit_point;
            
            glm::vec4 refracted_direction = getRefractedDirection(hit_info.incident_ray.direction, hit_info.normal, from_material->m_refractive_index, hit_info.phong_mat->m_refractive_index);
            glm::vec4 refracted_ray_origin = hit_info.incident_ray.origin + ((hit_info.t + 0.003) * hit_info.incident_ray.direction);
            
            
            if (hit_info.phong_mat->m_glossy_coefficients.x > 0) {
                if (GLOSSY_REFLECTION) {
                    glm::vec4 glossy_reflect_dir;
                    glm::vec3 glossy_reflect_color = glm::vec3(0);
                    counter++;
                    for (int i = 0 ; i < NUM_GLOSSY_REFLECTION_RAY ; ++i) {
                        glossy_reflect_dir = perturbe(reflected_direction, hit_info.normal, hit_info.phong_mat->m_glossy_coefficients.y);
                        Ray glossy_reflect_ray = Ray(intersection_point, glossy_reflect_dir);
                        glossy_reflect_color += rayColor(glossy_reflect_ray, lights, counter, PhongMaterial::Air, from_node);
                    }
                    glossy_reflect_color /= (double)NUM_GLOSSY_REFLECTION_RAY;
                    col += hit_info.phong_mat->m_glossy_coefficients.x * glossy_reflect_color * hit_info.phong_mat->m_ks;
                }
            } else {
                if (REFLECTION) {
                    counter++;
                    Ray reflected_ray = Ray(intersection_point, reflected_direction);
                    //                col += hit_info.phong_mat->m_ks * rayColor(reflected_ray, lights, counter, PhongMaterial::Air) * hit_info.phong_mat->m_shininess/120;
                    col += hit_info.phong_mat->m_ks * rayColor(reflected_ray, lights, counter, PhongMaterial::Air, from_node) * reflectance;
                    
                }
            }
            if (reflectance < 1.0) {
                if (hit_info.phong_mat->m_glossy_coefficients.z > 0) {
                    if (GLOSSY_REFRACTION){
                        glm::vec4 glossy_refract_dir;
                        glm::vec3 glossy_refract_color = glm::vec3(0);
                        counter++;
                        for (int i = 0 ; i < NUM_GLOSSY_REFRACTION_RAY ; ++i) {
                            glossy_refract_dir = perturbe(refracted_direction, -hit_info.normal, hit_info.phong_mat->m_glossy_coefficients.w);
                            Ray glossy_refract_ray = Ray(refracted_ray_origin, glossy_refract_dir);
                            glossy_refract_color += rayColor(glossy_refract_ray, lights, counter, hit_info.phong_mat, hit_info.node);
                        }
                        glossy_refract_color /= (double)NUM_GLOSSY_REFRACTION_RAY;
                        col += temp_phong->m_glossy_coefficients.z * glossy_refract_color  * (1 - reflectance);
                    }
                } else {
                    if (REFRACTION) {
                        Ray refracted_ray = Ray(refracted_ray_origin, refracted_direction);
                        
                        counter++;
                        col += (1 - reflectance) * rayColor(refracted_ray, lights, counter, hit_info.phong_mat, hit_info.node);
                        
                    }
                }
            }
        }
    } else {
//        int number = rand_int(1, 100);
//        if (number == 1)
//            col = glm::vec3(1,1,1);
//        else
//            col = glm::vec3(0,0,0);
    }
    
    return col;
    
}


Color directLight(const std::list<Light *>& lights, HitInformation& hit_info, int counter) {
    Color col = glm::vec3(0);
    Color col_sum = glm::vec3(0);
    
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
    int num_shadow_ray;
    
    double x = hit_info.t - EPSILON;
    
//    intersection_point = hit_info.incident_ray.pointAtParameterT(x);
    intersection_point = hit_info.incident_ray.origin + (x * hit_info.incident_ray.direction);
//    intersection_point = hit_info.hit_point;
    intersection_normal = (hit_info.normal);
    glm::vec3 kd, ks;
    
    kd = hit_info.phong_mat->m_kd;
    ks = hit_info.phong_mat->m_ks;
    
    
    for (const Light* light : lights) {
//        if (!light->is_point_source) {
//            const AreaLight* al = static_cast<const AreaLight*>(light);
//            std::cout << "area light with radius: " << al->radius  << std::endl;
//            continue;
//        }
        
        num_shadow_ray = light->is_point_source ? 1 : NUM_SHADOW_RAY;
        
        col_sum = glm::vec3(0);
        
        for (int i = 0 ; i < num_shadow_ray ; i++) {
            light_position = glm::vec4(light->getLightPosition(), 1);
            
            
//            std::cout << glm::to_string(light_position) << std::endl;
            
            shadow_ray_direction = (light_position - intersection_point);
            shadow_ray_origin = intersection_point;
            
            
            
            Ray shadow_ray = Ray(shadow_ray_origin, shadow_ray_direction);
            
            distance_to_light = glm::length(light_position - shadow_ray_origin);
            
            HitInformation shadow_ray_info = HitInformation(shadow_ray);
            hit(shadow_ray, m_root, shadow_ray_info);
            
            
            if (shadow_ray_info.hit) {
                distance_to_hit = glm::length(((shadow_ray_info.incident_ray.origin + (shadow_ray_info.t * shadow_ray_info.incident_ray.direction)) - shadow_ray_origin));
                if (distance_to_hit < distance_to_light) continue;
            }
            
            
            
            shadow_ray_direction = glm::normalize(shadow_ray_direction);
            intersection_normal = glm::normalize(intersection_normal);
            
            
            auto l_dot_n = glm::dot(shadow_ray_direction, intersection_normal) < 0 ? 0.0 : glm::dot(shadow_ray_direction, intersection_normal);
            
            Texture *texture = hit_info.getTexture();
            
            if (texture) {
                glm::vec2 uv = hit_info.getUV();
//                std::cout << "in direct light" << std::endl;
//                std::cout << "u: " << uv.x << " v: " << uv.y << std::endl;

                kd = texture->getColor(uv.x,1- uv.y);
            }
            
            if (texture){
                col_sum += kd * l_dot_n * light->colour;
            } else {
                if (kd != glm::vec3(0)) {
                    col_sum += kd * l_dot_n * light->colour;
                }
            }
            
            if(ks != glm::vec3(0)) {
//                reflected_ray_direction = (2 * l_dot_n * intersection_normal) - shadow_ray_direction;
//                view_direction = glm::normalize(-hit_info.incident_ray.direction);
//                auto r_dot_v = glm::dot(reflected_ray_direction, view_direction);
//                col_sum += ks * pow(r_dot_v, hit_info.phong_mat->m_shininess) * light->colour;
                glm::vec3 ri = glm::normalize(glm::vec3(hit_info.incident_ray.direction));
                glm::vec3 n = glm::normalize(glm::vec3(hit_info.normal));
                glm::vec3 rr = ri - 2 * n * glm::dot(ri, n);
                l_dot_n = glm::dot(rr, glm::normalize(glm::vec3(shadow_ray_direction))) > 0.0 ? glm::dot(rr, glm::normalize(glm::vec3(shadow_ray_direction))) : 0.0;
                double phong_coeff = std::pow(l_dot_n, hit_info.phong_mat->m_shininess);
                col_sum += phong_coeff * ks * light->colour;
            }
        }
        
        col += col_sum/num_shadow_ray;
        
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
    
//    glm::vec3 w = glm::normalize(glm::vec3(view - eye));
//    glm::vec3 u = -glm::normalize(glm::cross(w, up));
//    glm::vec3 v = glm::cross(u, w);
    
    glm::vec3 w, u, v;
    
    w = glm::normalize(view);
    u = glm::cross(glm::normalize(up), w);
    v = glm::cross(u, w);
    
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

void updateSceneGraph () {
    m_root->updateWorldMatrix(glm::mat4(1));
}

double calculateReflectance(glm::vec3 normal, glm::vec3 incident_ray_direction, double n_i, double n_t) {
    double cos_i, cos_t, sin_i_sq, sin_t_sq, n, rs, rp;
    
//    n = n_i/n_t;
//
//    cos_i = glm::dot(-incident_ray_direction, normal);
//    sin_i_sq = 1 - (cos_i * cos_i);
//    sin_t_sq = (n * n) * sin_i_sq;
//
//    if (sin_t_sq > 1) {
//        return 1.0;
//    }
//
//    cos_t = sqrt(1 - sin_t_sq);
//
//    rs = (n_t * cos_i - n_i * cos_t) / (n_t * cos_i + n_i * cos_t);
//    rp = (n_t * cos_t - n_i * cos_i) / (n_i * cos_i + n_t * cos_t);
    
    double r_0 = std::pow(((n_i - n_t) / (n_i + n_t)), 2.0);
    
    double a = std::pow((1 - glm::dot(-incident_ray_direction, normal)), 5);
    
//    return (rp * rp + rs * rs) / 2.0;
    double ret_val = r_0 + ((1 - r_0) * a);
    
    if (ret_val > 1.0) ret_val = 1.0;
    
    return ret_val;
}


glm::vec4 perturbe(const glm::vec4& ray_direction, const glm::vec4& normal, double exponent) {
//    double x1, x2, phi, cosine_theta, sine_theta, cosine_phi, sine_phi;
    
    glm::vec3 w = glm::normalize(glm::vec3(ray_direction));
    
    glm::vec3 e0 = glm::vec3 (0, 1, 0);
    glm::vec3 e1 = glm::vec3 (0, 0, 1);

    glm::vec3 u = glm::cross(w, e0);
    
    if (glm::length(u) < 0.1) {
        u = glm::cross(w, e1);
    }
    
    glm::vec3 v = glm::cross(w, u);
    
    glm::vec3 perturbed_direction;
    
    do {
        double x1 = getRandomDouble(0.0, 1.0);
        double x2 = getRandomDouble(0.0, 1.0);
        
//        std::cout << "X1: " << x1 << " X2: " << x2 << std::endl;
        double phi = x1 * 2 * glm::pi<double>();
        double cosine_theta = glm::pow(x2, (double)1.0/(exponent +1));
        double sine_theta = glm::sqrt(1 - (cosine_theta * cosine_theta));
        double cosine_phi = glm::cos(phi);
        double sine_phi = glm::sin(phi);
        
        perturbed_direction = w * cosine_theta + u * cosine_phi * sine_theta + v * sine_phi * sine_theta;
        
//        std::cout << "dot: " << glm::dot(perturbed_direction, glm::normalize(glm::vec3(normal))) << std::endl;
    } while (glm::dot(perturbed_direction, glm::vec3(normal)) <= 0);
    
    return glm::vec4(perturbed_direction,0);
}

double getRandomDouble (double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    
    return distribution(generator);
}

glm::vec4 getRefractedDirection(const glm::vec4& incident_direction, const glm::vec4& normal, double n_i, double n_t) {
//    double tir = 1 - (pow((n_i/n_t), 2) * (1 - pow(glm::dot(incident_direction, normal), 2)));
//
//    if (tir < 0) {
//        // return reflected ray
//    }
//
//    return (-(n_i/n_t) * glm::dot(incident_direction, normal) - sqrt(tir)) * normal + (n_i/n_t) * incident_direction;
    
//    Ray normalizedRay(ray.origin, glm::normalize(ray.direction));
    glm::vec4 normalized_direction = glm::normalize(incident_direction);
    glm::vec4 normalized_normal = glm::normalize(normal);
    double nr = n_i / n_t;
    
    double cosineTheta_i = -glm::dot(normalized_normal, normalized_direction);
    double sineTheta_i_t2 = 1 - cosineTheta_i * cosineTheta_i;
    double sineTheta_t_t2 = nr * nr * sineTheta_i_t2;
    
//    std::cout << "---------------------------" << std::endl;
//    std::cout << "Incoming refractive index: " << intersection.fromMaterial->m_refractive_index << std::endl;
//    std::cout << "Incoming angle: " << glm::degrees(glm::acos(cosineTheta_i)) << std::endl;
//    std::cout << "To refractive index: " << intersection.material->m_refractive_index << std::endl;
//    std::cout << "Refracted angle: " << glm::degrees(glm::asin(sqrt(sineTheta_t_t2))) << std::endl;
//#endif
//
//    if (sineTheta_t_t2 > 1) {
//        // Total internal reflection.
//#if DEBUG
//        std::cout << "TIR" << std::endl;
//#endif
//        return Ray({0,0,0,1}, {0,0,0,0});
//    }
    
    //    auto direction = (nr * cosineTheta_i - sqrt(1 - sineTheta_t_t2)) * intersection.normal - nr * normalizedRay.direction;
    auto direction = nr * normalized_direction + (nr * cosineTheta_i - sqrt(1 - sineTheta_t_t2)) * normalized_normal;
    direction = glm::normalize(direction);
//    assert(std::abs(direction.w) < EPSILON);
//    assert(intersection.t > 0);
//    glm::dvec4 hitpoint = ray.origin + ray.direction * intersection.t;
    
//    Ray r(hitpoint + EPSILON * direction, direction);
//#if DEBUG
//    std::cout << "Real refracted angle: " << glm::degrees( glm::acos(glm::dot(r.direction, -intersection.normal)) ) << std::endl;
//#endif
    
    return direction;
}


glm::vec4 getReflectedDirection(const glm::vec4& incident_direction, const glm::vec4& normal) {
    return incident_direction - (2 * glm::dot(incident_direction, normal) * normal);
}






