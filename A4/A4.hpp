#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
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

glm::vec4 pixelToWorld(double px, double py, glm::mat4& transformation);

glm::mat4 getPixelToWorldTransform(const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up, double fovy, double image_width, double image_height);
glm::mat4 T1(double image_width, double image_height, double distance);
glm::mat4 S2(double image_width, double image_height, double plane_width, double plane_height);
glm::mat4 R3(const glm::vec3 & eye, const glm::vec3 & view, const glm::vec3 & up);
glm::mat4 T4(const glm::vec3 & eye);

Color rayColor(const Ray& r, const std::list<Light *> & lights, int counter);
Color directLight(const std::list<Light *>& lights, HitInformation& hit_info, int counter);

void hit (const Ray& r, SceneNode* root, HitInformation& hit_info);


