#pragma once
#include <glm/glm.hpp>

namespace aie { class ShaderProgram; }

class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	virtual ~Mesh();

	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	void loadMaterial(const char* filename);
	void applyMaterial(aie::ShaderProgram* shader);

	void initialiseQuad();

	void initialise(unsigned int vertexCount, const Vertex* vertices,
		unsigned int indexCount = 0,
		unsigned int* indices = nullptr);

	void initialiseFromFile(const char* filename);

	virtual void draw();

protected:
	

	unsigned int triCount;
	unsigned int vao, vbo, ibo;

	glm::vec3 Ka; // ambient colour of the surface
	glm::vec3 Kd; // diffuse colour of the surface
	glm::vec3 Ks; // specular colour of the surface
	float specularPower; // tightness of specular highlights

};

