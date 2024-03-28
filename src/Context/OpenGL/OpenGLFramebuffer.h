#pragma once
#include "../../Renderer/Framebuffer.h"

class OpenGLFramebuffer : public Framebuffer {
    public:
        OpenGLFramebuffer(const FramebufferConfig& config, bool drawQuad = false);
        virtual ~OpenGLFramebuffer();

        const FramebufferConfig& GetConfig () const override { return m_config; }
        uint32_t GetColorAttachmentID() const override { return m_colorAttachment; }
        const std::shared_ptr<VertexArray> GetVertexArray() const override { return m_vertexArray;}
        const std::shared_ptr<Shader> GetShader() const override { return m_shader;}
        void Bind() override;
        void Unbind() override;
        void Create();
        void InitQuad();
    private:
        uint32_t m_ID, m_colorAttachment, m_depthStencilAttachment;
        FramebufferConfig m_config;
        std::shared_ptr<VertexArray> m_vertexArray = nullptr;
        std::shared_ptr<Shader> m_shader = nullptr;
};