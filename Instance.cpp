#include "Instance.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "GraphicsApplication.h"
#include "Scene.h"

Instance::Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader)
{
    m_transform = transform;
    m_mesh = mesh;
    m_shader = shader;
}

Instance::Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, Mesh* mesh, aie::ShaderProgram* shader)
{
    m_transform = makeTransform(position, eulerAngles, scale);
    m_mesh = mesh;
    m_shader = shader;
}


void Instance::draw(Scene* scene)
{
	// set the active shader
	m_shader->bind();

	// bind uniform variables in the shader
    glm::mat4 projectionViewModel = scene->getCamera()->GetProjectionMatrix(scene->getWindowSize().x, scene->getWindowSize().y) 
        * scene->getCamera()->GetViewMatrix() * m_transform;

    m_shader->bindUniform("ProjectionViewModel", projectionViewModel);
    m_shader->bindUniform("ModelMatrix", m_transform);
    m_shader->bindUniform("CameraPosition", scene->getCamera()->GetPosition());

    m_shader->bindUniform("AmbientColour", scene->getAmbientLightColour());
    m_shader->bindUniform("LightDirection", scene->getSunLight()->direction);
    m_shader->bindUniform("LightColour", scene->getSunLight()->getColour());

    int numLights = scene->getNumLights();
    m_shader->bindUniform("NumLights", numLights);
    m_shader->bindUniform("PointLightColour", numLights, scene->getPointLightColours());
    m_shader->bindUniform("PointLightPosition", numLights, scene->getPointLightPositions());

    m_mesh->applyMaterial(m_shader);

    // draw mesh
    m_mesh->draw();
    
}

glm::mat4 Instance::makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
    return glm::translate(glm::mat4(1), position)
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), scale);
}
