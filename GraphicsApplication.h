#pragma once
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class GraphicsApplication : public Application
{
public:
	GraphicsApplication() { s_instance = this; }

	// overridden singleton getter
	static GraphicsApplication* get() { return s_instance; }

	virtual bool Startup() override;
	virtual bool Update() override;
	virtual void Draw() override;
	virtual void Shutdown() override;

	glm::vec2 GetMousePosition() { return m_mousePosition; }
	glm::vec2 GetMouseDelta() { return m_mousePosition - m_lastMousePosition; }

	static void SetMousePosition(GLFWwindow* window, double x, double y)
	{
		s_instance->m_mousePosition = glm::vec2((float)x, (float)y);
	}


protected:

	// overridden static instance
	static GraphicsApplication* s_instance;

	aie::ShaderProgram      m_shader;
	Mesh                    m_renderObjectMesh;
	glm::mat4               m_renderObjectTransform;

	GLFWwindow* m_window;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;

	Camera m_camera;

	const int windowWidth = 1280;
	const int windowHeight = 720;
};

