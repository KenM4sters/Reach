#pragma once
#include "../Core.h"
#include "Shader.h"
#include "VertexArray.h"



struct FramebufferConfig {
    uint32_t Samples = 1;
    uint32_t Width, Height;
};

class Framebuffer {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual const FramebufferConfig& GetConfig() const = 0;
        virtual const uint32_t& GetColorAttachmentID() const = 0;
        virtual const std::shared_ptr<VertexArray> GetVertexArray() const {return nullptr;}
        virtual const std::shared_ptr<Shader> GetShader() const {return nullptr;}

        virtual void SetConfig(FramebufferConfig config) = 0;

        
        static std::shared_ptr<Framebuffer> Create(FramebufferConfig& config, bool drawQuad = false);
};