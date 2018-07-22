#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "GeometryNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "HitInformation.hpp"


typedef glm::vec3 Color;

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
);

glm::vec4 pixelToWorld(glm::vec3 px_pos, glm::mat4& transformation);

glm::mat4 getPixelToWorldTransform(const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up, double fovy, double image_width, double image_height);
glm::mat4 T1(double image_width, double image_height, double distance);
glm::mat4 S2(double image_width, double image_height, double plane_width, double plane_height);
glm::mat4 R3(const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up);
glm::mat4 T4(const glm::vec3 & eye);

Color rayColor(const Ray& r, const std::list<Light *> & lights, int counter, PhongMaterial* from_material, SceneNode* from_node);
Color directLight(const std::list<Light *>& lights, HitInformation& hit_info, int counter,PhongMaterial* phong);

double calculateReflectance(glm::vec3 normal, glm::vec3 incident_ray_direction, double n_i, double n_t);
glm::vec4 perturbe(const glm::vec4& ray_direction, const glm::vec4& normal, double exponent);

double getRandomDouble (double min, double max);

glm::vec4 getRefractedDirection(const glm::vec4& incident_direction, const glm::vec4& normal, double n_i, double n_t);
glm::vec4 getReflectedDirection(const glm::vec4& incident_direction, const glm::vec4& normal);

void hit (const Ray& r, SceneNode* root, HitInformation& hit_info);
int rand_int(int min, int max);

void updateSceneGraph();
