#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application
{
public:

	virtual bool Startup();
	virtual bool Update();
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;

protected:

	double m_prevFrameTime = 0.0f;
	float m_deltaTime = 0.0f;
};

