#include "Texture.h"
#include "Renderer.h"
#include "../Context/OpenGL/OpenGLTexture.h"

std::shared_ptr<Texture2D> Texture2D::Create(const char* path, const char* name) 
{
    switch(Renderer::m_rendererAPI->GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - Renderer::m_rendererAPI::API is currently set to VOID!");
            break;
        case API::OPEN_GL:
            return std::make_shared<Texture2D>(OpenGLTexture2D(path, name));
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;   
    }
}

void Texture2D::Bind() 
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Bind(uint32_t& count) 
{
    glActiveTexture(GL_TEXTURE0 + count);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind() 
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Unbind(uint32_t& count) 
{
    glActiveTexture(GL_TEXTURE0 + count);
    glBindTexture(GL_TEXTURE_2D, 0);
}
