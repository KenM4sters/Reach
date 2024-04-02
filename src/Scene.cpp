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
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    uint32_t window_width = app.GetWindow()->GetWindowProps().Width;
    uint32_t window_height = app.GetWindow()->GetWindowProps().Height;
    m_camera = std::make_shared<PerspectiveCamera>(camera_pos, window_width, window_height);

    // Model Container
    m_models = std::make_shared<std::vector<std::shared_ptr<Model>>>();

    // Backpack Model
    auto backpack_shader = Shader::Create("backpack_shader", "src/Shaders/PHONG_textured.vert", "src/Shaders/PHONG_textured.frag");
    auto backpack_model = std::make_shared<Model>(
        "Assets/Models/backpack/backpack.obj",
        OBJECT_TYPE::MODEL, 
        new Material(backpack_shader)
    );
    backpack_model->GetTransformProps()->Scale = glm::vec3(0.25f);
    backpack_model->GetTransformProps()->Translation = glm::vec3(-5.0f, 0.0f, 0.0f);
    m_models->push_back(backpack_model);
    
    // Sphere Model
    auto sphere_shader = Shader::Create("sphere_shader", "src/Shaders/PBR_bare.vert", "src/Shaders/PBR_bare.frag");
    auto sphere_model = std::make_shared<Model>(
        "Assets/Models/Sphere/sphere.obj", 
        OBJECT_TYPE::MODEL, 
        new Material(sphere_shader)
    );
    sphere_model->GetTransformProps()->Scale = glm::vec3(0.05f);
    sphere_model->GetTransformProps()->Translation = glm::vec3(45.0f, 0.0f, 0.0f);
    m_models->push_back(sphere_model);
     
    // Light
    m_pointLight = new PointLight(OBJECT_TYPE::LIGHT, new PointLightProps());
    m_pointLight->GetTransformProps()->Translation = glm::vec3(-5.0f, 3.0f, 2.0f);
    m_pointLight->GetLightProps()->Intensity = 0.2f;


    

    // Background Mesh
    auto background_shader = Shader::Create("background_shader", "src/Shaders/Background.vert", "src/Shaders/Background.frag");
    auto vertices = MakeVertexFromFloat(cube_vertices);
    auto background_vao = VertexArray::Create(
        VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex))
    );
    m_background = std::make_shared<Mesh>(
        background_vao,
        new Material(background_shader)
    );
    
    // Background Environment
    m_background->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/ocean.hdr", "ocean_tex"));
    m_background->GetMaterial()->GetProps()->CubeTexture = CubeTexture::Create(512, 512);
    auto fbo_config = FramebufferConfig({1, 512, 512});
    m_backgroundFBO = Framebuffer::Create(fbo_config, FramebufferType::SKELETON);
    Renderer::PrepareBackground(
        m_background, 
        m_backgroundFBO, 
        Shader::Create("env_shader", "src/Shaders/EqToCube.vert", "src/Shaders/EqToCube.frag")
    );
    glViewport(0, 0, 1600, 1200); // PrepareBackground changes the viewport, so we need to reset it.
}

void Scene::OnDetach()  
{

}

void Scene::Update()  
{
    HandleUserInput();
    Renderer::CreateBackground(m_background, m_backgroundFBO, m_camera);
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
        std::string Albedo = name + "Albedo";
        std::string Metallic = name + "Metallic";
        std::string roughness = name + "Roughness";
        std::string AO = name + "AO";
        
        ImGui::Text(name.c_str());
        ImGui::DragFloat3(pos_name.c_str(), (float*)(&model->GetTransformProps()->Translation), 0.01f);
        ImGui::DragFloat3(scale_name.c_str(), (float*)(&model->GetTransformProps()->Scale), 0.01f, 0.0f);
        ImGui::Text("Material");
        ImGui::DragFloat3(Albedo.c_str(), (float*)(&model->GetMaterial()->GetProps()->Albedo), 0.01f, 1.0f);
        ImGui::DragFloat(Metallic.c_str(), (float*)(&model->GetMaterial()->GetProps()->Metallic), 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat(roughness.c_str(), (float*)(&model->GetMaterial()->GetProps()->Roughness), 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat(AO.c_str(), (float*)(&model->GetMaterial()->GetProps()->AO), 0.01f, 0.0f, 1.0f);
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
