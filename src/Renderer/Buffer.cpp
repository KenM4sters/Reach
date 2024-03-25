#include "Buffer.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(std::vector<Vertex>& vertices, uint32_t size) 
{
    switch(Renderer::m_rendererAPI->GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - RendererAPI is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return new OpenGLVertexBuffer(vertices, size);
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }

}
IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) 
{
    switch(Renderer::m_rendererAPI->GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - Renderer::m_rendererAPI::API is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return new OpenGLIndexBuffer(indices, size);
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;   
    }
}
