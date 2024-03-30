#include "Framebuffer.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLFramebuffer.h"


std::shared_ptr<Framebuffer> Framebuffer::Create(FramebufferConfig& config, bool drawQuad) 
{
    switch(Renderer::m_rendererAPI->GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - Renderer::m_rendererAPI::API is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return std::make_shared<OpenGLFramebuffer>(config, drawQuad);
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;   
    }
}