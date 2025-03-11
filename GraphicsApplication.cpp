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

bool GraphicsApplication::startup()
{
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

    // Initialize gizmos and camera
    Gizmos::create(10000, 10000, 0, 0);
    m_view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);


    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool GraphicsApplication::update()
{
    m_deltaTime = glfwGetTime() - m_prevFrameTime;
    m_prevFrameTime = glfwGetTime();

    return glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void GraphicsApplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Game Logic and Render Code

    Gizmos::clear();

    // Add 3-axis transform visual to the origin
    Gizmos::addTransform(glm::mat4(1));

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    // Add gridlines to the xy plane
    for (int i = 0; i < 21; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, 0, 10),
            vec3(-10 + i, 0, -10),
            i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i),
            vec3(-10, 0, -10 + i),
            i == 10 ? white : black);
    }

    static mat4 sunTransform = mat4(1);
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
    Gizmos::addSphere(vec3(0), 0.2f, 10, 10, vec4(0.9f, 0.9f, 0.9f, 0.9f), &moonGlobalTransform);

    Gizmos::draw(m_projection * m_view);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GraphicsApplication::shutdown()
{
    Gizmos::destroy();
    glfwTerminate();
}
