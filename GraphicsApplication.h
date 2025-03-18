#pragma once
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"

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

	aie::ShaderProgram      m_simpleShader;
	aie::ShaderProgram      m_phongShader;
	Mesh                    m_renderObjectMesh;
	glm::mat4               m_renderObjectTransform;
	aie::Texture			m_renderObjectTexture;

	GLFWwindow* m_window;

	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;

	struct Light {
		glm::vec3 direction;
		glm::vec3 diffuseColour;
		glm::vec3 specularColour;
	};

	Light m_light;
	glm::vec3 m_ambientLightColour;

	Camera m_camera;

	const int windowWidth = 1280;
	const int windowHeight = 720;
};

