#include "Skybox.h"
#include "Renderer/Renderer.h"
#include "App.h"

Skybox::Skybox(std::shared_ptr<Texture2D> HDR, std::shared_ptr<Shader> skyboxShader)
    : m_skyboxShader(skyboxShader)
{
    auto vertices = MakeVertexFromFloat(cube_vertices);
    auto background_vao = VertexArray::Create(VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex)));
    m_mesh = std::make_shared<Mesh>(background_vao, new Material(skyboxShader));

    m_mesh->GetMaterial()->GetProps()->Textures.push_back(HDR);
    m_mesh->GetMaterial()->GetProps()->CubeTexture = CubeTexture::Create(512*2, 512*2, true);

    m_convolutedCubeMap = CubeTexture::Create(32*2, 32*2, false);
    m_prefilteredCubeMap = CubeTexture::Create(128*2, 128*2, true);
    m_BRDFTexture = Texture2D::Create(512*2, 512*2, 2);

    PrepareFramebuffer();
    PrepareTextures();
    ResetChanges();
}

Skybox::~Skybox() 
{

}

void Skybox::PrepareFramebuffer() 
{
    auto fbo_config = FramebufferConfig({1, 512, 512});
    m_FBO = Framebuffer::Create(fbo_config, FramebufferType::SKELETON);
}

void Skybox::PrepareTextures() 
{
    auto convolutionShader = Shader::Create("convolutionShader", "src/Shaders/Convolution.vert", "src/Shaders/Convolution.frag");
    auto eqToCubeShader = Shader::Create("env_shader", "src/Shaders/EqToCube.vert", "src/Shaders/EqToCube.frag");
    auto prefilterShader = Shader::Create("prefilter_shader", "src/Shaders/Prefilter.vert", "src/Shaders/Prefilter.frag");
    auto BRDF_shader = Shader::Create("BRDF_shader", "src/Shaders/BRDF.vert", "src/Shaders/BRDF.frag");

    auto lookUpTexture = Texture2D::Create(512, 512, 2); // Skeleton texture (no pre-loaded data).
    
    auto& cube_tex = m_mesh->GetMaterial()->GetProps()->CubeTexture;
    auto& hdri_tex = m_mesh->GetMaterial()->GetProps()->Textures[0];

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

    // Conversion from a HDR equirectangular map to a cube map equivalent.
    eqToCubeShader->Use();
    eqToCubeShader->SetInt("equirectangularMap", 0);
    eqToCubeShader->SetMat4f("projection", captureProjection);
    hdri_tex->Bind();
    cube_tex->Bind();

    m_FBO->Bind();
    glViewport(0, 0, 512*2, 512*2);
    for (unsigned int i = 0; i < 6; ++i)
    {
        eqToCubeShader->SetMat4f("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_tex->GetID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::Draw(m_mesh->GetVAO()); // Draws VAO without using any shaders or textures.
    }
    cube_tex->Bind();        
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Clean up.
    glBindBuffer(GL_FRAMEBUFFER, 0);
    hdri_tex->Unbind();
    eqToCubeShader->Release();
    cube_tex->Unbind();


    // Convolute the cube map.
    auto convolution_config = FramebufferConfig({1, 32, 32});
    m_FBO->SetConfig(convolution_config);

    convolutionShader->Use();
    convolutionShader->SetInt("environmentMap", 0);
    convolutionShader->SetMat4f("projection", captureProjection);
    m_FBO->Bind();
    cube_tex->Bind();
    glViewport(0, 0, 32*2, 32*2); // don't forget to configure the viewport to the capture dimensions.
    for (unsigned int i = 0; i < 6; ++i)
    {
        convolutionShader->SetMat4f("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_convolutedCubeMap->GetID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::Draw(m_mesh->GetVAO()); // Draws VAO without using any shaders or textures.
    }

    // Clean up.
    glBindBuffer(GL_FRAMEBUFFER, 0);
    cube_tex->Unbind();
    convolutionShader->Release();


    // Prefilter Map

    // Generate Mip Map.
    m_prefilteredCubeMap->Bind();
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    m_prefilteredCubeMap->Unbind();

    prefilterShader->Use();
    prefilterShader->SetInt("environmentMap", 0);
    prefilterShader->SetMat4f("projection", captureProjection);
    m_FBO->Bind();
    cube_tex->Bind();
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        uint32_t mipWidth  = static_cast<uint32_t>(128 * std::pow(0.5, mip));
        uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));

        auto config = FramebufferConfig({1, mipWidth, mipHeight});
        m_FBO->SetConfig(config);
        glViewport(0, 0, mipWidth*2, mipHeight*2);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->SetFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader->SetMat4f("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilteredCubeMap->GetID(), mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            Renderer::Draw(m_mesh->GetVAO()); // Draws VAO without using any shaders or textures.
        }
    }
    // Cleanup.
    prefilterShader->Release();
    m_FBO->Unbind();
    cube_tex->Unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // BRDF

    // Look Up texture
    auto quad_vertices = MakeVertexFromFloat(square_vertices);
    auto vao = VertexArray::Create(
        VertexBuffer::Create(quad_vertices, quad_vertices.size()*sizeof(Vertex)),
        IndexBuffer::Create(square_indices, sizeof(square_indices)*sizeof(float))
    );
    auto brdf_config = FramebufferConfig({1, 512, 512});
    m_FBO->SetConfig(brdf_config);

    // App& app = App::GetInstance();

    m_FBO->Bind();
    BRDF_shader->Use();
    glViewport(0, 0, 512*2, 512*2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFTexture->GetID(), 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Renderer::Draw(vao);

    // Cleanup.
    m_FBO->Unbind();
    BRDF_shader->Release();
    m_BRDFTexture->Unbind();

    // while(app.GetWindow()->IsRunning()) 
    // {
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     Renderer::Draw(vao);
    //     Renderer::m_rendererAPI->ListenToEvents();
    //     Renderer::m_rendererAPI->SwapBuffers(app.GetWindow()->GetNativeWindow());
    // }


    // Optional - Convoluted texture can look pretty cool if you don't want an obvious background.
    // m_mesh->GetMaterial()->GetProps()->CubeTexture = m_prefilteredCubeMap;
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Blends the colors around each side of the cube map.
}

void Skybox::ResetChanges() 
{
    glViewport(0, 0, 1600, 1200);
}
