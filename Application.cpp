#include "Application.h"

bool Application::Startup()
{
	return true;
}

bool Application::Update()
{
	m_deltaTime = glfwGetTime() - m_prevFrameTime;
	m_prevFrameTime = glfwGetTime();

	return true;
}

void Application::Shutdown()
{
}
