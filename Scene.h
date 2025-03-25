#pragma once
#include <list>
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"

class Camera;
class Instance;


const int MAX_LIGHTS = 4;

class Scene
{
public:
	Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLightColour);
	~Scene();

	void AddInstance(Instance* instance);
	void draw();

	Camera* getCamera() { return m_camera; }
	glm::vec2 getWindowSize() { return m_windowSize; }
	Light* getSunLight() { return &m_sunLight; }
	glm::vec3 getAmbientLightColour() { return m_ambientLightColour; }
	int getNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* getPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointLightColours() { return &m_pointLightColours[0]; }

	std::vector<Light>& getPointLights() { return m_pointLights; }

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_sunLight;
	glm::vec3 m_ambientLightColour;

	std::vector<Light> m_pointLights;
	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];
	
	std::list<Instance*> m_instances;
};

