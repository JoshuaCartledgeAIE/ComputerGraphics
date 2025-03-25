#pragma once
#include <glm/glm.hpp>

struct Light {
	Light() : direction(glm::vec3(0, -1, 0)), baseColour(glm::vec3(1, 1, 1)), intensity(50) {}
	Light(glm::vec3 pos, glm::vec3 colour, float intens) {
		position = pos;
		intensity = intens;
		baseColour = colour;
	}

	glm::vec3 getColour() { return baseColour * intensity; }

	glm::vec3* getBaseColour() { return &baseColour; }

	void setColour(glm::vec3 colour) { baseColour = colour; }

	// either this is a directional light with a direction or this is a point light with a position
	union { glm::vec3 direction; glm::vec3 position; };
	float intensity;

protected:
	glm::vec3 baseColour;
};
