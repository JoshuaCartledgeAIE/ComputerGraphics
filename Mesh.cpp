#include "Mesh.h"
#include <gl/GL.h>

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void Mesh::initialiseQuad()
{
}

void Mesh::draw()
{
}
