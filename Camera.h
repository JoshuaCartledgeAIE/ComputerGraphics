#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class Camera
{
public:
	Camera() : m_theta(0), m_phi(0), m_position(glm::vec3(0)) {}
	Camera(float theta, float phi, glm::vec3 pos) : m_theta(theta), m_phi(phi), m_position(pos) {}

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	void Update(float deltaTime, GLFWwindow* window);

	glm::vec3 GetPosition() { return m_position; }

private:
	float m_theta;
	float m_phi;
	glm::vec3 m_position;

	float m_camMoveSpeed = 3.0f;
	float m_camTurnSpeed = 0.1f;
	float m_fov = 45.0f;
};

