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

void Renderer::PrepareBackground(std::shared_ptr<Mesh> mesh, std::shared_ptr<Framebuffer> FBO, std::shared_ptr<Shader> eqToCubeShader, std::shared_ptr<CubeTexture> convolutedTex) 
{

    auto convolutionShader = Shader::Create("convolutionShader", "src/Shaders/Convolution.vert", "src/Shaders/Convolution.frag");
    auto cube_tex = mesh->GetMaterial()->GetProps()->CubeTexture;
    auto hdri_tex = mesh->GetMaterial()->GetProps()->Textures[0];

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 captureViews[6] = 
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // convert HDR equirectangular environment map to cubemap equivalent
    eqToCubeShader->Use();
    eqToCubeShader->SetInt("equirectangularMap", 0);
    eqToCubeShader->SetMat4f("projection", captureProjection);
    hdri_tex->Bind();
    cube_tex->Bind();

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    FBO->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        eqToCubeShader->SetMat4f("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_tex->GetID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto& VAO = mesh->GetVAO();
        VAO->Bind();
        if(VAO->GetIndexBuffer())
            m_rendererAPI->DrawIndexed(VAO);
        else
            m_rendererAPI->Draw(VAO);
        VAO->Unbind();
    }
    glBindBuffer(GL_FRAMEBUFFER, 0);
    hdri_tex->Unbind();
    eqToCubeShader->Release();
    cube_tex->Unbind();


    // Convolute the cube map
    auto convolution_config = FramebufferConfig({1, 32, 32});
    FBO->SetConfig(convolution_config);

    convolutionShader->Use();
    convolutionShader->SetInt("environmentMap", 0);
    convolutionShader->SetMat4f("projection", captureProjection);
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    FBO->Bind();
    cube_tex->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        convolutionShader->SetMat4f("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, convolutedTex->GetID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto& VAO = mesh->GetVAO();
        VAO->Bind();
        if(VAO->GetIndexBuffer())
            m_rendererAPI->DrawIndexed(VAO);
        else
            m_rendererAPI->Draw(VAO);
        VAO->Unbind();
    }
    glBindBuffer(GL_FRAMEBUFFER, 0);
    cube_tex->Unbind();
    convolutionShader->Release();

    // Optional - Convulted texture can look pretty cool if you don't want an obvious background.
    // mesh->GetMaterial()->GetProps()->CubeTexture = convolutedTex;

}

void Renderer::CreateBackground(std::shared_ptr<Mesh> mesh,std::shared_ptr<Framebuffer> FBO, std::shared_ptr<PerspectiveCamera> camera) 
{
    auto shader = mesh->GetMaterial()->GetShader();
    auto texture = mesh->GetMaterial()->GetProps()->CubeTexture;
    texture->Bind();
    shader->SetInt("environmentMap", 0);
    shader->SetMat4f("projection", camera->GetProjectionMatrix());
    shader->SetMat4f("view", camera->GetViewMatrix());
    Renderer::Submit(mesh);
    texture->Unbind();

}

void Renderer::CreateScene() 
{

}

void Renderer::EndScene() 
{

}
