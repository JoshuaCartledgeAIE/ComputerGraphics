#include "Mesh.h"
#include "glad.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <vector>
#include "Shader.h"
#include <ios>
#include <fstream>
#include <sstream>

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void Mesh::loadMaterial(const char* filename)
{
    std::fstream file(filename, std::ios::in);
    std::string line;
    std::string header;
    char buffer[256];
    while (!file.eof())
    {
        file.getline(buffer, 256);
        line = buffer;
        std::stringstream ss(line, std::stringstream::in | std::stringstream::out);
        if (line.find("Ka") == 0)
            ss >> header >> Ka.x >> Ka.y >> Ka.z;
        if (line.find("Ks") == 0)
            ss >> header >> Ks.x >> Ks.y >> Ks.z;
        if (line.find("Kd") == 0)
            ss >> header >> Kd.x >> Kd.y >> Kd.z;
        if (line.find("Ns") == 0)
            ss >> header >> specularPower;
    }

}

void Mesh::applyMaterial(aie::ShaderProgram* shader)
{
    shader->bindUniform("Ka", Ka);
    shader->bindUniform("Kd", Kd);
    shader->bindUniform("Ks", Ks);
    shader->bindUniform("SpecularPower", specularPower);
}

void Mesh::initialiseQuad()
{
    // check to ensure mesh has not yet been initialised already
    assert(vao == 0);

    // generate buffers and vertex arrays
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // bind vertex array and vertex buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // define 6 vertices for 2 triangles
    Vertex vertices[6];
    vertices[0].position = { -0.5f, 0, 0.5f, 1 };
    vertices[1].position = { 0.5f, 0, 0.5f, 1 };
    vertices[2].position = { -0.5f, 0, -0.5f, 1 };

    vertices[3].position = { -0.5f, 0, -0.5f, 1 };
    vertices[4].position = { 0.5f, 0, 0.5f, 1 };
    vertices[5].position = { 0.5f, 0, -0.5f, 1 };

    for (int i = 0; i <= 5; i++) {
        vertices[i].normal = { 0, 1, 0, 0 };
    }

    // fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // enable the position and normal attributes of the Vertex struct
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    triCount = 2;
}

void Mesh::initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
    // check to ensure mesh has not yet been initialised already
    assert(vao == 0);

    // generate buffers and vertex arrays
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // bind vertex array and vertex buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // fill vertex buffer
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    // enable the position and normal attributes of the Vertex struct
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

    // check if there are indices, and bind them if so
    if (indexCount != 0)
    {
        glGenBuffers(1, &ibo);

        // bind vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // fill vertex buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        triCount = indexCount / 3;
    }
    else {
        triCount = vertexCount / 3;
    }

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initialiseFromFile(const char* filename)
{
    // read vertices from the model
    const aiScene* scene = aiImportFile(filename, 0);

    // use first mesh that was found
    aiMesh* mesh = scene->mMeshes[0];

    // extract indices from mesh
    int numFaces = mesh->mNumFaces;
    std::vector<unsigned int> indices;

    for (int i = 0; i < numFaces; i++)
    {
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        // generate a second triangle for quads
        if (mesh->mFaces[i].mNumIndices == 4)
        {
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[3]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }

    // extract vertex data
    int numV = mesh->mNumVertices;
    Vertex* vertices = new Vertex[numV];
    for (int i = 0; i < numV; i++)
    {
        vertices[i].position = glm::vec4(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y, 
            mesh->mVertices[i].z, 1);
        vertices[i].normal = glm::vec4(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z, 1);
        // TODO UVs
    }
    initialise(numV, vertices, indices.size(), indices.data());

    delete[] vertices;
}

void Mesh::draw()
{
    glBindVertexArray(vao);
    // check if using indices or just vertices?
    if (ibo != 0)
        glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
}
