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

void Renderer::PrepareScene(std::shared_ptr<Mesh>& mesh, std::shared_ptr<PerspectiveCamera>* camera) 
{
    auto shader = mesh->GetMaterial()->GetShader();
    shader->Use();
    shader->SetMat4f("projection", camera->get()->GetProjectionMatrix());
    shader->SetMat4f("view", camera->get()->GetViewMatrix());
    shader->SetMat4f("model", mesh->GetTransformProps()->ModelMatrix);

    auto props = mesh->GetMaterial()->GetProps();
    auto textures = props->Textures;
    shader->SetVec3f("material.Ambient", props->Ambient);
    shader->SetVec3f("material.Diffuse", props->Diffuse);
    shader->SetVec3f("material.Specular", props->Specular);
    shader->SetFloat("material.Shininess", props->Shininess);

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
