#include "Renderer.h"
#include "../Context/OpenGL/OpenGLRendererAPI.h"
#include "../Context/OpenGL/OpenGLTexture.h"
#include "../Context/OpenGL/OpenGLFramebuffer.h"

// Out of class initializer for renderer api - OpenGL by default.
RendererAPI* Renderer::m_rendererAPI = new OpenGLRendererAPI();

void Renderer::Submit(const std::shared_ptr<Framebuffer>& fbo) 
{
    fbo->GetVertexArray()->Bind();
    fbo->GetShader()->Use();
    glBindTexture(GL_TEXTURE_2D, fbo->GetColorAttachmentID());
    m_rendererAPI->DrawIndexed(fbo->GetVertexArray());
    fbo->GetVertexArray()->Unbind();
    fbo->GetShader()->Release();
    glBindTexture(GL_TEXTURE_2D, 0);
}

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

void Renderer::PrepareScene(std::shared_ptr<std::vector<std::shared_ptr<Model>>>& models, std::shared_ptr<PerspectiveCamera>* camera, Light* light) 
{
    for(const auto& model : *models) 
    {
        for(auto mesh : *model->GetMeshes()) 
        {
            uint16_t diffuse_count = 1;
            uint16_t specular_count = 1;
            uint16_t normal_count = 1;
            uint16_t height_count = 1;
            auto& textures = mesh.GetMaterial()->GetProps()->Textures;
            auto shader = mesh.GetMaterial()->GetShader();
            for(uint32_t i = 0; i < textures.size(); i++) {
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
            // Material Props (not really needed if the model has textures, but definitiely needed if it doesn't).
            auto mat_props = mesh.GetMaterial()->GetProps();
            shader->SetVec3f("material.Ambient", mat_props->Ambient);
            shader->SetVec3f("material.Diffuse", mat_props->Diffuse);
            shader->SetVec3f("material.Specular", mat_props->Specular);
            shader->SetFloat("material.Shininess", mat_props->Shininess);
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
            model_transforms->ModelMatrix = glm::scale(model_transforms->ModelMatrix, model_transforms->Scale);
            model_transforms->ModelMatrix = glm::translate(model_transforms->ModelMatrix, model_transforms->Translation);
            mesh.SetTransformProps(model->GetTransformProps());
            shader->SetMat4f("model", mesh.GetTransformProps()->ModelMatrix);

            // Finally submit each mesh of our model for rendering.
            const auto& ptr = std::make_shared<Mesh>(mesh);
            Renderer::Submit(ptr);
            // Cleanup.
            glActiveTexture(GL_TEXTURE0);
            shader->Release();
        }
    }
}

void Renderer::CreateScene() 
{

}

void Renderer::EndScene() 
{

}
