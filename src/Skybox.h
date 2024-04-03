#pragma once
#include "Core.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Mesh.h"


class Skybox {
    public:
        Skybox(std::shared_ptr<Texture2D> HDR, std::shared_ptr<Shader> skyboxShader);
        ~Skybox();
        void PrepareFramebuffer();
        void PrepareTextures();
        void ResetChanges();

        // Getters
        inline std::shared_ptr<Shader> GetSkyboxShader() const { return m_skyboxShader; }
        inline std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
        inline std::shared_ptr<Framebuffer> GetFramebuffer() const { return m_FBO; }
    public:
        std::shared_ptr<CubeTexture> m_convolutedCubeMap = nullptr;
        std::shared_ptr<CubeTexture> m_rawCubeMap = nullptr;
        std::shared_ptr<CubeTexture> m_prefilteredCubeMap = nullptr;
    private:
        std::shared_ptr<Shader> m_skyboxShader = nullptr;
        std::shared_ptr<Mesh> m_mesh = nullptr;
        std::shared_ptr<Framebuffer> m_FBO = nullptr;

};

