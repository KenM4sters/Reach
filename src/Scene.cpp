#include "Scene.h"
#include "Context/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"
#include "App.h"
#include <ImGui/imgui.h>
#include "Mesh.h"
#include "Light.h"

Scene::Scene()
    : Layer("Scene")
{

}

Scene::~Scene() 
{

}

void Scene::OnAttach()  
{   
    App& app = App::GetInstance();
    m_window = app.GetWindow()->GetNativeWindow();

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    uint32_t window_width = app.GetWindow()->GetWindowProps().Width;
    uint32_t window_height = app.GetWindow()->GetWindowProps().Height;
    m_camera = std::make_shared<PerspectiveCamera>(camera_pos, window_width, window_height);
    std::string name = "model_shader";
    m_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(name, "src/Shaders/model.vert", "src/Shaders/model.frag"));
    // Model
    auto model_transforms = new TransformProps();
    m_model = std::make_shared<Model>("Assets/Models/backpack/backpack.obj", m_shader, OBJECT_TYPE::MODEL, model_transforms);


    // Light
    m_pointLight = new PointLight(OBJECT_TYPE::LIGHT, new TransformProps(), new PointLightProps());
    m_pointLight->GetTransformProps()->Translation = glm::vec3(3.0f, 3.0f, 2.0f);
}

void Scene::OnDetach()  
{

}

void Scene::Update()  
{
    HandleUserInput();

    Renderer::PrepareScene(m_model, &m_camera, m_pointLight);
    Renderer::Submit(m_model);
}

void Scene::UpdateInterface()  
{
    ImGui::Text("Mesh");
    ImGui::DragFloat3("MeshPosition", (float*)(&m_model->GetTransformProps()->Translation));
    ImGui::Text("Light");
    ImGui::ColorEdit4("AmbientColor", (float*)(&m_pointLight->GetLightProps()->AmbientColor));
    ImGui::DragFloat("Intensity", (float*)(&m_pointLight->GetLightProps()->Intensity), 0.01f);
    ImGui::DragFloat3("LightPosition", (float*)(&m_pointLight->GetTransformProps()->Translation));

}

void Scene::HandleUserInput() 
{
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) 
        m_camera->HandleUserInput(TransformDirection::FORWARD, false);
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) 
        m_camera->HandleUserInput(TransformDirection::LEFT, false);
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) 
        m_camera->HandleUserInput(TransformDirection::BACKWARD, false);
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) 
        m_camera->HandleUserInput(TransformDirection::RIGHT, false);
    if(glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) 
        m_camera->HandleUserInput(TransformDirection::UP, false);
    if(glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) 
        m_camera->HandleUserInput(TransformDirection::DOWN, false);
}
