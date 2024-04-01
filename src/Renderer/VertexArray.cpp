#include "VertexArray.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLVertexArray.h"


std::shared_ptr<VertexArray> VertexArray::Create(VertexBuffer* vertex_buffer, IndexBuffer* index_buffer) 
{
    switch(RendererAPI::GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexArray::Create() - RendererAPI is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return std::make_shared<OpenGLVertexArray>(vertex_buffer, index_buffer);
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexArray::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }
}
std::shared_ptr<VertexArray> VertexArray::Create(VertexBuffer* vertex_buffer) 
{
    switch(RendererAPI::GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexArray::Create() - RendererAPI is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return std::make_shared<OpenGLVertexArray>(vertex_buffer);
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexArray::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }
}

