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

    // Mesh
    std::vector<Vertex> vertices = MakeVertexFromFloat(cube_vertices);
    auto vbo = VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex));
    // auto ebo = IndexBuffer::Create(square_indices, sizeof(square_indices));
    vao = static_cast<std::shared_ptr<VertexArray>>(VertexArray::Create(vbo));
    std::string name = "quad_shader";
    m_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(name, "src/Shaders/quad.vert", "src/Shaders/quad.frag"));
    auto mat = new Material(m_shader);
    auto props = mat->GetProps();
    auto tex = Texture2D::Create("Assets/Textures/sfiii.png", "test");
    props->Textures.push_back(tex);
    m_mesh = std::make_shared<Mesh>(vao, new Material(m_shader), new TransformProps());

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

    Renderer::PrepareScene(m_mesh, &m_camera, m_pointLight);
    Renderer::Submit(m_mesh);
}

void Scene::UpdateInterface()  
{
    ImGui::Text("Mesh");
    ImGui::ColorEdit4("Color", (float*)(&m_mesh->GetMaterial()->GetProps()->Ambient));
    ImGui::DragFloat3("Diffuse", (float*)(&m_mesh->GetMaterial()->GetProps()->Diffuse));
    ImGui::DragFloat3("Specular", (float*)(&m_mesh->GetMaterial()->GetProps()->Specular));
    ImGui::DragFloat("Shininess", (float*)(&m_mesh->GetMaterial()->GetProps()->Shininess));
    ImGui::DragFloat3("MeshPosition", (float*)(&m_mesh->GetTransformProps()->Translation));
    ImGui::Text("Light");
    ImGui::ColorEdit4("AmbientColor", (float*)(&m_pointLight->GetLightProps()->AmbientColor));
    ImGui::DragFloat3("Intensity", (float*)(&m_pointLight->GetLightProps()->Intensity));
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
