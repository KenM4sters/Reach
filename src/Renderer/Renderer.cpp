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

void Renderer::PrepareScene(std::shared_ptr<Model>& model, std::shared_ptr<PerspectiveCamera>* camera, Light* light) 
{
    uint16_t diffuse_count = 1;
    uint16_t specular_count = 1;
    uint16_t normal_count = 1;
    uint16_t height_count = 1;

    for(auto mesh : *model->GetMeshes()) 
    {
        auto& textures = mesh.GetMaterial()->GetProps()->Textures;
        auto shader = mesh.GetMaterial()->GetShader();
        for(uint32_t i = 0; i < textures.size(); i++) 
        {
            std::string tex_index;
            std::string name = textures[i]->GetName();
            if(name == "texture_diffuse") {
                tex_index = std::to_string(diffuse_count++);
            } else if(name == "texture_specular") {
                tex_index = std::to_string(specular_count++);
            } else if(name == "texture_normal") {
                tex_index = std::to_string(normal_count++);
            } else if(name == "texture_height") {
                tex_index = std::to_string(height_count++);
            }
            shader->Use();
            textures[i]->Bind(i);
            shader->SetInt(name + tex_index, i);
        };
        // Camera props
        shader->SetVec3f("CameraPos", camera->get()->GetPosition());
        shader->SetMat4f("projection", camera->get()->GetProjectionMatrix());
        shader->SetMat4f("view", camera->get()->GetViewMatrix());
        // Light props
        shader->SetVec3f("light.Position", light->GetTransformProps()->Translation);
        shader->SetVec3f("light.AmbientColor", light->GetLightProps()->AmbientColor);
        shader->SetFloat("light.Intensity", light->GetLightProps()->Intensity);
        // Transform props
        auto model_transforms = model->GetTransformProps();
        model_transforms->ModelMatrix = glm::mat4(1.0f);
        model_transforms->ModelMatrix = glm::translate(model_transforms->ModelMatrix, model_transforms->Translation);
        mesh.SetTransformProps(model->GetTransformProps());
        shader->SetMat4f("model", mesh.GetTransformProps()->ModelMatrix);
    }
}

void Renderer::CreateScene() 
{

}

void Renderer::EndScene() 
{

}
