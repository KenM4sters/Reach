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
    auto backpack_shader = Shader::Create("Backpack_shader", "src/Shaders/PHONG_textured.vert", "src/Shaders/PHONG_textured.frag");
    auto backpack_model = std::make_shared<Model>(
        "Assets/Models/backpack/backpack.obj",
        OBJECT_TYPE::MODEL, 
        new Material(backpack_shader)
    );
    backpack_model->GetTransformProps()->Scale = glm::vec3(0.25f);
    backpack_model->GetTransformProps()->Translation = glm::vec3(-5.0f, 0.0f, 0.0f);
    m_models->push_back(backpack_model);
    
    // Bare Sphere Model
    auto sphere_shader = Shader::Create("Sphere_shader", "src/Shaders/PBR_bare.vert", "src/Shaders/PBR_bare.frag");
    auto sphere_model = std::make_shared<Model>(
        "Assets/Models/Sphere/sphere.obj", 
        OBJECT_TYPE::MODEL, 
        new Material(sphere_shader)
    );
    sphere_model->GetTransformProps()->Scale = glm::vec3(0.05f);
    sphere_model->GetTransformProps()->Translation = glm::vec3(45.0f, 0.0f, 0.0f);
    m_models->push_back(sphere_model);

    // Textured Sphere Model
    auto textured_sphere_shader = Shader::Create("texturedSphere_shader", "src/Shaders/PBR_textured.vert", "src/Shaders/PBR_textured.frag");
    auto textured_sphere_model = std::make_shared<Model>(
        "Assets/Models/Sphere/sphere.obj", 
        OBJECT_TYPE::MODEL, 
        new Material(textured_sphere_shader)
    );
    textured_sphere_model->GetTransformProps()->Scale = glm::vec3(0.05f);
    textured_sphere_model->GetTransformProps()->Translation = glm::vec3(0.0f, 0.0f, 0.0f);

    textured_sphere_model->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/white-marble-bl/white-marble_albedo.png", "texture_diffuse"));
    textured_sphere_model->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/white-marble-bl/white-marble_height.png", "texture_height"));
    textured_sphere_model->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/white-marble-bl/white-marble_normal-ogl.png", "texture_normal"));
    textured_sphere_model->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/white-marble-bl/white-marble_metallic.png", "texture_metallic"));
    textured_sphere_model->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/white-marble-bl/white-marble_roughness.png", "texture_roughness"));
    textured_sphere_model->GetMaterial()->GetProps()->Textures.push_back(Texture2D::Create("Assets/Textures/white-marble-bl/white-marble_ao.png", "texture_ao"));

    m_models->push_back(textured_sphere_model);

    // Cerberus Model
    auto cerberus_shader = Shader::Create("texturedSphere_shader", "src/Shaders/PBR_textured.vert", "src/Shaders/PBR_textured.frag");
    auto cerberus_model = std::make_shared<Model>(
        "Assets/Models/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX", 
        OBJECT_TYPE::MODEL, 
        new Material(cerberus_shader)
    );
    cerberus_model->GetTransformProps()->Scale = glm::vec3(0.01f);
    m_models->push_back(cerberus_model);
     
    // Light
    m_pointLight = new PointLight(OBJECT_TYPE::LIGHT, new PointLightProps());
    m_pointLight->GetTransformProps()->Translation = glm::vec3(-5.0f, 3.0f, 2.0f);
    m_pointLight->GetLightProps()->Intensity = 4.0f;

    // Background Environment
    m_skybox = std::make_shared<Skybox>(
        Texture2D::Create("Assets/Textures/ocean.hdr", "ocean"),
        Shader::Create("background_shader", "src/Shaders/Background.vert", "src/Shaders/Background.frag")
    );

    // Give all models the convoluted map for shading.
    backpack_model->GetMaterial()->GetProps()->CubeTexture = m_skybox->m_convolutedCubeMap;
    sphere_model->GetMaterial()->GetProps()->CubeTexture = m_skybox->m_convolutedCubeMap;
    textured_sphere_model->GetMaterial()->GetProps()->CubeTexture = m_skybox->m_convolutedCubeMap;
    cerberus_model->GetMaterial()->GetProps()->CubeTexture = m_skybox->m_convolutedCubeMap;

    // Cube map generation is all done now, so we can set our camera back to a more appropriate place.
    glm::vec3 new_camera_pos = glm::vec3(0.0f, 0.0f, 5.0f);
    m_camera->SetPosition(new_camera_pos);
    
}

void Scene::OnDetach()  
{

}

void Scene::Update()  
{
    HandleUserInput();
    Renderer::CreateBackground(m_skybox, m_camera);
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
