#include "VertexArray.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLVertexArray.h"


VertexArray* VertexArray::Create(VertexBuffer* vertex_buffer, IndexBuffer* index_buffer) 
{
    switch(Renderer::GetAPI()) 
    {
        case RendererAPI::VOID:
            throw std::runtime_error("ERROR::VertexArray::Create() - RendererAPI is currently set to VOID!");
            break;
        case RendererAPI::OPEN_GL:
            return new OpenGLVertexArray(vertex_buffer, index_buffer);
            break;
        case RendererAPI::VULKAN:
            throw std::runtime_error("Error::VertexArray::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }
}