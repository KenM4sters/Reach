#include "Renderer.h"
#include "../Context/OpenGL/OpenGLRendererAPI.h"

// Out of class initializer for renderer api - OpenGL by default.
RendererAPI* Renderer::m_rendererAPI = new OpenGLRendererAPI();

void Renderer::Submit(const std::shared_ptr<Mesh>& mesh) 
{   
    const auto& VAO = mesh->GetVAO();
    VAO->Bind();
    mesh->GetMaterial()->GetShader()->Use();
    if(VAO->GetIndexBuffer())
        m_rendererAPI->DrawIndexed(VAO);
    else
        m_rendererAPI->Draw(VAO);
    VAO->Unbind();
    mesh->GetMaterial()->GetShader()->Release();
}

void Renderer::Submit(const std::shared_ptr<Model>& model) 
{   
    for(auto mesh : *model->GetMeshes()) {
        mesh.GetMaterial()->GetShader()->Use();
        mesh.GetVAO()->Bind();
        m_rendererAPI->DrawIndexed(mesh.GetVAO());
        mesh.GetVAO()->Unbind();
        mesh.GetMaterial()->GetShader()->Release();
    }
}

void Renderer::PrepareScene(std::shared_ptr<Mesh>& mesh, std::shared_ptr<PerspectiveCamera>* camera, Light* light) 
{
    auto shader = mesh->GetMaterial()->GetShader();
    auto mesh_transforms = mesh->GetTransformProps();
    auto& mesh_model = mesh_transforms->ModelMatrix;
    mesh_model = glm::mat4(1.0f);
    mesh_model = glm::scale(mesh_model, mesh_transforms->Scale);
    mesh_model = glm::translate(mesh_model, mesh_transforms->Translation);

    shader->Use();
    shader->SetMat4f("projection", camera->get()->GetProjectionMatrix());
    shader->SetMat4f("view", camera->get()->GetViewMatrix());
    shader->SetMat4f("model", mesh_transforms->ModelMatrix);

    auto mesh_props = mesh->GetMaterial()->GetProps();
    auto point_light = dynamic_cast<PointLight*>(light);
    auto light_props = point_light->GetLightProps();
    auto transform_props = point_light->GetTransformProps();

    auto textures = mesh_props->Textures;
    // Mesh
    shader->SetVec3f("material.Ambient",mesh_props->Ambient);
    shader->SetVec3f("material.Diffuse",mesh_props->Diffuse);
    shader->SetVec3f("material.Specular",mesh_props->Specular);
    shader->SetFloat("material.Shininess",mesh_props->Shininess);
    // Light
    shader->SetVec3f("light.Position", transform_props->Translation);
    shader->SetVec3f("light.Ambient",light_props->AmbientColor);
    shader->SetFloat("light.Intensity",light_props->Intensity);
    // Camera
    shader->SetVec3f("CameraPos", camera->get()->GetPosition());

    for(uint32_t i = 0; i < textures.size(); i++) 
    {
        textures[i]->Bind(i);
        shader->SetInt("material." + textures[i]->GetName(), i);
    }

    shader->Release();
    for(uint32_t i = 0; i < textures.size(); i++) 
    {
        textures[i]->Unbind(i);
    }
}

void Renderer::CreateScene() 
{

}

void Renderer::EndScene() 
{

}
