#pragma once
#include <glm/glm.hpp>

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

	void initialiseQuad();

	void initialise(unsigned int vertexCount, const Vertex* vertices,
		unsigned int indexCount = 0,
		unsigned int* indices = nullptr);

	void initialiseFromFile(const char* filename);

	virtual void draw();

protected:
	

	unsigned int triCount;
	unsigned int vao, vbo, ibo;
};

