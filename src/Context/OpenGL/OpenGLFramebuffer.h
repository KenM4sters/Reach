#pragma once
#include "../../Renderer/Framebuffer.h"

class OpenGLFramebuffer : public Framebuffer {
    public:
        OpenGLFramebuffer(FramebufferConfig& config, FramebufferType type, bool drawQuad = false);
        virtual ~OpenGLFramebuffer();

        void Bind() override;
        void Unbind() override;
        void Create();
        void CreateSkeleton();
        void InitQuad();

        // Getters
        const FramebufferConfig& GetConfig () const override { return m_config; }
        const uint32_t& GetColorAttachmentID() const override { return m_colorAttachment; }
        const uint32_t& GetDepthStencilAttachmentID() const override {return m_depthStencilAttachment; };
        const uint32_t& GetID() const override { return m_ID; }
        const std::shared_ptr<VertexArray> GetVertexArray() const override { return m_vertexArray;}
        const std::shared_ptr<Shader> GetShader() const override { return m_shader;}
        // Setters
        void SetConfig(FramebufferConfig config) override;
    private:
        uint32_t m_ID, m_colorAttachment, m_depthStencilAttachment;
        FramebufferConfig m_config;
        std::shared_ptr<VertexArray> m_vertexArray = nullptr;
        std::shared_ptr<Shader> m_shader = nullptr;
        FramebufferType m_type;
};