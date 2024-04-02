#include "Renderer.h"
#include "../Context/OpenGL/OpenGLRendererAPI.h"
#include "../Context/OpenGL/OpenGLTexture.h"
#include "../Context/OpenGL/OpenGLFramebuffer.h"
#include "../App.h"

// Out of class initializer for renderer api - OpenGL by default.
RendererAPI* Renderer::m_rendererAPI = new OpenGLRendererAPI();

void Renderer::Submit(const std::shared_ptr<Framebuffer>& fbo) 
{
    fbo->GetShader()->Use();
    fbo->GetVertexArray()->Bind();

    glActiveTexture(fbo->GetColorAttachmentID());
    glBindTexture(GL_TEXTURE_2D, fbo->GetColorAttachmentID());
    m_rendererAPI->DrawIndexed(fbo->GetVertexArray());
    
    fbo->GetVertexArray()->Unbind();
    fbo->GetShader()->Release();  
    // std::cout << fbo->GetColorAttachmentID() << std::endl;
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

void Renderer::Draw(const std::shared_ptr<VertexArray> vao) 
{
    vao->Bind();
    if(vao->GetIndexBuffer())
        m_rendererAPI->DrawIndexed(vao);
    else
        m_rendererAPI->Draw(vao);
    vao->Unbind();
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
            auto& cube_texture = model->GetMaterial()->GetProps()->CubeTexture;
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
                } else {
                    tex_index = "";
                }
                shader->Use();
                textures[i]->Bind(i);
                shader->SetInt(name + tex_index, i);
            };
            // Add the environemt map.
            cube_texture->Bind();
            shader->SetInt("env_map", textures.size());

            // Material Props (not really needed if the model has textures, but definitiely needed if it doesn't).
            auto mat_props = mesh.GetMaterial()->GetProps();
            auto model_props = model->GetMaterial()->GetProps();
            mesh.GetMaterial()->GetProps()->Albedo = model_props->Albedo;
            mesh.GetMaterial()->GetProps()->Metallic = model_props->Metallic;
            mesh.GetMaterial()->GetProps()->Roughness = model_props->Roughness;
            mesh.GetMaterial()->GetProps()->AO = model_props->AO;
            shader->SetVec3f("material.Albedo", mat_props->Albedo);
            shader->SetFloat("material.Metallic", mat_props->Metallic);
            shader->SetFloat("material.Roughness", mat_props->Roughness);
            shader->SetFloat("material.AO", mat_props->AO);
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
            cube_texture->Unbind();
            shader->Release();
        }
    }
}

void Renderer::CreateBackground(std::shared_ptr<Skybox> skybox, std::shared_ptr<PerspectiveCamera> camera) 
{
    auto shader = skybox->GetMesh()->GetMaterial()->GetShader();
    auto texture = skybox->GetMesh()->GetMaterial()->GetProps()->CubeTexture;
    texture->Bind();
    shader->SetInt("environmentMap", 0);
    shader->SetMat4f("projection", camera->GetProjectionMatrix());
    shader->SetMat4f("view", camera->GetViewMatrix());
    Renderer::Submit(skybox->GetMesh());
    texture->Unbind();

}

void Renderer::CreateScene() 
{

}

void Renderer::EndScene() 
{

}
