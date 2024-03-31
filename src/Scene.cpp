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
    // Camera
    App& app = App::GetInstance();
    m_window = app.GetWindow()->GetNativeWindow();
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    uint32_t window_width = app.GetWindow()->GetWindowProps().Width;
    uint32_t window_height = app.GetWindow()->GetWindowProps().Height;
    m_camera = std::make_shared<PerspectiveCamera>(camera_pos, window_width, window_height);

    // Model Container
    m_models = std::make_shared<std::vector<std::shared_ptr<Model>>>();

    // Backpack Model
    std::string backpack_name = "backpack_shader";
    auto backpack_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(backpack_name, "src/Shaders/textured_model.vert", "src/Shaders/textured_model.frag"));
    auto backpack_transforms = new TransformProps();
    auto backpack_model = std::make_shared<Model>("Assets/Models/backpack/backpack.obj", backpack_shader, OBJECT_TYPE::MODEL, backpack_transforms);
    backpack_model->GetTransformProps()->Scale = glm::vec3(0.25f);
    m_models->push_back(backpack_model);

    // Sphere Model
    std::string sphere_name = "sphere_shader";
    auto sphere_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(sphere_name, "src/Shaders/bare_model.vert", "src/Shaders/bare_model.frag"));
    auto sphere_transforms = new TransformProps();
    auto sphere_model = std::make_shared<Model>("Assets/Models/Sphere/sphere.obj", sphere_shader, OBJECT_TYPE::MODEL, sphere_transforms);
    sphere_model->GetTransformProps()->Scale = glm::vec3(0.1f);
    m_models->push_back(sphere_model);
     

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

    Renderer::PrepareScene(m_models, &m_camera, m_pointLight);
}

void Scene::UpdateInterface()  
{
    ImGui::Text("Light");
    ImGui::ColorEdit4("AmbientColor", (float*)(&m_pointLight->GetLightProps()->AmbientColor));
    ImGui::DragFloat("Intensity", (float*)(&m_pointLight->GetLightProps()->Intensity), 0.01f);
    ImGui::DragFloat3("LightPosition", (float*)(&m_pointLight->GetTransformProps()->Translation), 0.01f);

    for(auto& model : *m_models) 
    {
        const std::string& name = model->GetName();
        std::string pos_name = name + "Position";
        std::string scale_name = name + "Scale";
        ImGui::Text(name.c_str());
        ImGui::DragFloat3(pos_name.c_str(), (float*)(&model->GetTransformProps()->Translation), 0.01f);
        ImGui::DragFloat3(scale_name.c_str(), (float*)(&model->GetTransformProps()->Scale), 0.01f);
    }

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
