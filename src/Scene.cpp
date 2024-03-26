#include "Scene.h"
#include "Context/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"
#include "App.h"
#include <ImGui/imgui.h>

static ImVec4 color = ImVec4(1.0f, 0.5, 0.0f, 1.0f);

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

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, -3.0f);
    uint32_t window_width = app.GetWindow()->GetWindowProps().Width;
    uint32_t window_height = app.GetWindow()->GetWindowProps().Height;
    m_camera = std::make_shared<PerspectiveCamera>(camera_pos, window_width, window_height);



    std::vector<Vertex> vertices = MakeVertexFromFloat(square_vertices);
    auto vbo = VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex));
    auto ebo = IndexBuffer::Create(square_indices, sizeof(square_indices));
    vao = static_cast<std::shared_ptr<VertexArray>>(VertexArray::Create(vbo, ebo));

    std::string name = "quad_shader";
    m_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(name, "src/Shaders/quad.vert", "src/Shaders/quad.frag"));
}

void Scene::OnDetach()  
{

}

void Scene::Update()  
{
    Renderer::PrepareScene();
    m_shader->Use();
    m_shader->SetVec4f("uColor", glm::vec4(color.x, color.y, color.z, color.w));
    Renderer::Submit(vao, m_shader);
}

void Scene::UpdateInterface()  
{
    if(ImGui::Button("Change Color")) 
    {
        color = ImVec4(0.4f, 0.8f, 1.0f, 1.0f);
    }
}
