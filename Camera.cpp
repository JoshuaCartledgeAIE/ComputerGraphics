#include "Camera.h"
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include "GraphicsApplication.h"
#include <algorithm>

glm::mat4 Camera::GetViewMatrix()
{
    // convert both angles to radians
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    // get xyz vector of direction the camera is pointing (based on theta (left/right) and phi (up/down) angles)
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), sin(thetaR) * cos(phiR));

    // return rotation of looking at that forward vector, up direction being vertically up
    return glm::lookAt(m_position, m_position + forward, glm::vec3(0,1,0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::radians(m_fov),
        w / h,
        0.1f, 1000.f);
}

void Camera::Update(float deltaTime, GLFWwindow* window)
{
    // convert both angles to radians
    float thetaR = glm::radians(m_theta);
    float phiR = glm::radians(m_phi);

    // get forward, right, up axes of the camera based on theta and phi
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), sin(thetaR) * cos(phiR));
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    glm::vec3 up(0,1,0);

    m_camMoveSpeed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? 10.0f : 3.0f;

    if (glfwGetKey(window, GLFW_KEY_X)) {
        m_position += up * deltaTime * m_camMoveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Z)) {
        m_position -= up * deltaTime * m_camMoveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        m_position += forward * deltaTime * m_camMoveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        m_position -= forward * deltaTime * m_camMoveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        m_position += right * deltaTime * m_camMoveSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        m_position -= right * deltaTime * m_camMoveSpeed;
    }

    glm::vec2 mouseDelta = GraphicsApplication::get()->GetMouseDelta();

    m_camTurnSpeed = 0.1f;
    // if right mouse button is pressed, change values of theta and phi according to mouse movement
    if (glfwGetMouseButton(window, 1))
    {
        m_theta += m_camTurnSpeed * mouseDelta.x;
        m_phi -= m_camTurnSpeed * mouseDelta.y;
        // clamp phi to prevent gimbal lock (when phi is -90 or 90)
        m_phi = std::max(-85.f, std::min(m_phi, 85.f));
    }
}
