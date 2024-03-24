#include "Buffer.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLBuffer.h"

VertexBuffer* VertexBuffer::Create(std::vector<Vertex>& vertices, uint32_t size) 
{
    switch(Renderer::GetAPI()) 
    {
        case RendererAPI::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - RendererAPI is currently set to VOID!");
            break;
        case RendererAPI::OPEN_GL:
            return new OpenGLVertexBuffer(vertices, size);
            break;
        case RendererAPI::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }

}
IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) 
{
    switch(Renderer::GetAPI()) 
    {
        case RendererAPI::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - RendererAPI is currently set to VOID!");
            break;
        case RendererAPI::OPEN_GL:
            return new OpenGLIndexBuffer(indices, size);
            break;
        case RendererAPI::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;   
    }
}
