#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh;
namespace aie {
	class ShaderProgram;
}
class Scene;

class Instance
{
public:
	Instance() {};
	Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader);
	Instance(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale, Mesh* mesh, aie::ShaderProgram* shader);
	~Instance() {};


	void draw(Scene* scene);

	glm::mat4 makeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
	
	void setTransform(glm::mat4 newTransform) { m_transform = newTransform; }
	glm::mat4 getTransform() { return m_transform; }

protected:
	glm::mat4 m_transform;
	Mesh* m_mesh;
	aie::ShaderProgram* m_shader;
};

