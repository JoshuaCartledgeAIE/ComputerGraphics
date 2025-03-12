#include "GraphicsApplication.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

using aie::Gizmos;

GraphicsApplication* GraphicsApplication::s_instance;

bool GraphicsApplication::Startup()
{
    Application::Startup();

    if (glfwInit() == false)
        return false;

    // Create OpenGL window
    m_window = glfwCreateWindow(windowWidth, windowHeight, "Rendering Engine", nullptr, nullptr);

    if (m_window == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL()) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    // check OpenGL version
    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    // Initialize OpenGL background colour
    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);

    // Initialize gizmos and camera
    Gizmos::create(10000, 10000, 0, 0);
    m_camera = Camera(225, -45, vec3(20, 20, 20));
    glfwSetCursorPosCallback(m_window, &GraphicsApplication::SetMousePosition);


    // Load shader
    m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
    m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

    if (m_shader.link() == false) {
        printf("Shader Error: %s\n", m_shader.getLastError());
        return false;
    }

    m_renderObjectMesh.initialiseFromFile("./stanford/Bunny.obj");

    // make the quad 10 units wide
    m_renderObjectTransform = {
          0.5f,0,0,0,
          0,0.5f,0,0,
          0,0,0.5f,0,
          0,0,0,1 };

    

    return true;
}

bool GraphicsApplication::Update()
{
    Application::Update();

    m_camera.Update(m_deltaTime, m_window);

    m_lastMousePosition = m_mousePosition;

    return glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void GraphicsApplication::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Game Logic and Render Code

    Gizmos::clear();

    // Add 3-axis transform visual to the origin
    Gizmos::addTransform(glm::mat4(1));

    glm::mat4 projectionViewMatrix = m_camera.GetProjectionMatrix(windowWidth, windowHeight) * m_camera.GetViewMatrix();

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    int gridlineCount = 1000;

    // Add gridlines to the xy plane
    for (int i = 0; i < gridlineCount; i++)
    {
        Gizmos::addLine(vec3(-gridlineCount/2.0f + i, 0, gridlineCount / 2.0f),
            vec3(-gridlineCount / 2.0f + i, 0, -gridlineCount / 2.0f),
            i == gridlineCount / 2.0f ? white : black);

        Gizmos::addLine(vec3(gridlineCount / 2.0f, 0, -gridlineCount / 2.0f + i),
            vec3(-gridlineCount / 2.0f, 0, -gridlineCount / 2.0f + i),
            i == gridlineCount / 2.0f ? white : black);
    }

    /*static mat4 sunTransform = mat4(1);
    static mat4 planetTransform = glm::translate(mat4(1), vec3(0, 0, 6));
    static mat4 moonTransform = glm::translate(mat4(1), vec3(0, 0, 2));

    vec3 planetUpAxis = vec3(1, 1, 0);

    sunTransform = glm::rotate(sunTransform, m_deltaTime * 0.5f, vec3(0, 1, 0));
    planetTransform = glm::rotate(planetTransform, m_deltaTime * 1.0f, planetUpAxis);
    moonTransform = glm::rotate(moonTransform, m_deltaTime * 5.0f, vec3(0, 1, 0));

    mat4 planetGlobalTransform = sunTransform * planetTransform;
    mat4 moonGlobalTransform = planetGlobalTransform * moonTransform;

    Gizmos::addSphere(vec3(0), 2, 10, 10, vec4(0.8f, 0.6f, 0.2f, 0.9f), &sunTransform);
    Gizmos::addSphere(vec3(0), 1, 10, 10, vec4(0.2f, 0.6f, 0.8f, 0.9f), &planetGlobalTransform);
    Gizmos::addSphere(vec3(0), 0.2f, 10, 10, vec4(0.9f, 0.9f, 0.9f, 0.9f), &moonGlobalTransform);*/

    Gizmos::draw(projectionViewMatrix);

    // bind shader
    m_shader.bind();

    m_renderObjectTransform = glm::rotate(m_renderObjectTransform, m_deltaTime * 1.0f, vec3(0, 1, 0));

    // bind transform
    mat4 projectionViewModel = projectionViewMatrix * m_renderObjectTransform;
    m_shader.bindUniform("ProjectionViewModel", projectionViewModel);

    // draw quad
    m_renderObjectMesh.draw();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GraphicsApplication::Shutdown()
{
    Gizmos::destroy();
    glfwTerminate();
}
