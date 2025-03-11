#pragma once
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"

class GraphicsApplication : public Application
{
public:
	virtual bool startup() override;
	virtual bool update() override;
	virtual void draw() override;
	virtual void shutdown() override;

protected:

	aie::ShaderProgram      m_shader;
	Mesh                    m_quadMesh;
	glm::mat4               m_quadTransform;

	const int windowWidth = 1280;
	const int windowHeight = 720;
};

