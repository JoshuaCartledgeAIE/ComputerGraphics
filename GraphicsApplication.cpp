#include "GraphicsApplication.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>
#include "Instance.h"
#include "Scene.h"
#include <string>

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
    Gizmos::create(100000, 100000, 0, 0);
    m_camera = Camera(225, -30, vec3(20, 20, 20));
    glfwSetCursorPosCallback(m_window, &GraphicsApplication::SetMousePosition);

    // Initialize ImGUI
    ImGui_ImplGlfwGL3_Init(m_window, true);

    // Load shaders
    aie::ShaderProgram* phongShader = new aie::ShaderProgram;
    if (phongShader->loadAllShaderStages("./shaders/phong_with_normal_map.vert", "./shaders/phong_with_normal_map.frag") == false)
        return false;

    // Load objects' meshes and materials
    Mesh* soulspearMesh = new Mesh();
    soulspearMesh->initialiseFromFile("./soulspear.obj");
    soulspearMesh->loadMaterial("./soulspear.mtl");
    

    Light sunLight;
    sunLight.direction = vec3(-0.5f, -1, -0.5f);
    sunLight.setColour({1, 1, 1});
    sunLight.intensity = 1;
    vec3 ambientLightColour = { 0.5f, 0.5f, 0.5f };

    m_scene = new Scene(&m_camera, glm::vec2(windowWidth, windowHeight), sunLight, ambientLightColour);

    // add 10 soulspears in a row, with varying rotations
    for (int i = 0; i < 10; i++)
        m_scene->AddInstance(new Instance(vec3(i*8 - 40, 0, 0), vec3(0, i*10 - 50, 0), vec3(3, 3, 3), soulspearMesh, phongShader));

    // add red point light on left side
    m_scene->getPointLights().push_back(Light(vec3(5,3,5), vec3(1,0,0), 100));

    // add blue point light on right side
    m_scene->getPointLights().push_back(Light(vec3(-5, 3, 5), vec3(0, 0, 1), 100));

    return true;
}

bool GraphicsApplication::Update()
{
    Application::Update();

    ImGui_ImplGlfwGL3_NewFrame();

    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &m_scene->getSunLight()->direction[0], 0.01f, -1.0f,
        1.0f);
    ImGui::DragFloat3("Sunlight Colour", &m_scene->getSunLight()->getBaseColour()[0][0], 0.01f, 0.0f,
        1.0f);
    ImGui::DragFloat("Sunlight Intensity", &m_scene->getSunLight()->intensity, 0.01f, 0.0f,
        10.0f);

    ImGui::Checkbox("Debug Show Point Lights", &m_renderDebugPointLights);
    int i = 0;
    for (auto&& light : m_scene->getPointLights()) 
    {
        i++;
        ImGui::PushID(i);
        ImGui::TextUnformatted(std::string("Point Light ").append(std::to_string(i).append(":")).c_str());
        ImGui::DragFloat3("Position", &light.position[0], 0.2f, -100.0f,
            100.0f);
        ImGui::DragFloat3("Colour", &light.getBaseColour()[0][0], 0.01f, 0.0f,
            1.0f);
        ImGui::DragFloat("Intensity", &light.intensity, 0.05f, 0.0f,
            1000.0f);
        ImGui::PopID();
    }
    
    ImGui::End();

    m_camera.Update(m_deltaTime, m_window);

    m_lastMousePosition = m_mousePosition;

    // rotate light direction
    //m_light.direction = glm::normalize(vec3(glm::cos(glfwGetTime() * 2), glm::sin(glfwGetTime() * 2), 0));

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

    // render sun's direction at the origin
    Gizmos::addLine(vec3(0,0,0), m_scene->getSunLight()->direction * 10.0f, vec4(m_scene->getSunLight()->getColour(), 1));

    // render point lights if debug rendering is turned on
    if (m_renderDebugPointLights) 
    {
        for (int i = 0; i < m_scene->getNumLights(); i++)
        {
            Light light = m_scene->getPointLights()[i];
            Gizmos::addSphere(light.position, sqrt(light.intensity), 10, 10, vec4(*light.getBaseColour(), 0.1f));
        }
    }
    


   

    // Draw the instance
    m_scene->draw();

    Gizmos::draw(projectionViewMatrix);

    ImGui::Render();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GraphicsApplication::Shutdown()
{
    delete m_scene;
    ImGui_ImplGlfwGL3_Shutdown();
    Gizmos::destroy();
    glfwTerminate();
}
