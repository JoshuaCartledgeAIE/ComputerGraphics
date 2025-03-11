#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application
{
public:
	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;

protected:
	glm::mat4 m_view;
	glm::mat4 m_projection;

	GLFWwindow* m_window;

	double m_prevFrameTime = 0.0f;
	float m_deltaTime = 0.0f;
};

