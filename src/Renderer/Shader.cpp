#include "Shader.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLShader.h"

std::shared_ptr<Shader> Shader::Create(std::string name, const char* vert_src, const char* frag_src, const char* geo_src) 
{
    switch(RendererAPI::GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexArray::Create() - RendererAPI is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return std::make_shared<OpenGLShader>(name, vert_src, frag_src, geo_src);
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexArray::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }
}