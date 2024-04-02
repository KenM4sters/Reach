#include "OpenGLFramebuffer.h"
#include "../../Renderer/VertexArray.h"
#include "OpenGLShader.h"

OpenGLFramebuffer::OpenGLFramebuffer(FramebufferConfig& config, FramebufferType type, bool drawQuad)
    : m_config(config), m_type(type)
{
    switch(type) 
    {
        case FramebufferType::SKELETON:
            CreateSkeleton();
            break;
        case FramebufferType::COLOR_STENCIL_DEPTH:
            Create();
            break;
        case FramebufferType::COLOR:
            break;
    }

    if(drawQuad) InitQuad();
}
OpenGLFramebuffer::~OpenGLFramebuffer() 
{
    glDeleteFramebuffers(1, &m_ID);
}

void OpenGLFramebuffer::Create() 
{
    glGenFramebuffers(1, &m_ID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glGenTextures(1, &m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 2 * m_config.Width, 2 * m_config.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    glGenRenderbuffers(1, &m_depthStencilAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 2 * m_config.Width, 2 * m_config.Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilAttachment);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::Failed to create frame buffer!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLFramebuffer::CreateSkeleton() 
{
    glGenFramebuffers(1, &m_ID);
    glGenRenderbuffers(1, &m_depthStencilAttachment);

    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 2 * m_config.Width, 2 * m_config.Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilAttachment);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::Failed to create frame buffer!" << std::endl;
    }
}

void OpenGLFramebuffer::InitQuad() 
{
    std::vector<Vertex> vertices = MakeVertexFromFloat(square_vertices);
    auto vbo = VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex));
    auto ebo = IndexBuffer::Create(square_indices, sizeof(square_indices));
    m_vertexArray = static_cast<std::shared_ptr<VertexArray>>(VertexArray::Create(vbo, ebo));
    
    std::string name = "fbo_shader";
    m_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(name, "src/Shaders/fbo_shader.vert", "src/Shaders/fbo_shader.frag"));

}

void OpenGLFramebuffer::Bind() 
{ 
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLFramebuffer::Unbind() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
}

void OpenGLFramebuffer::SetConfig(FramebufferConfig config) 
{
    m_config = config;
    switch(m_type) 
    {
        case FramebufferType::SKELETON:
            CreateSkeleton();
            break;
        case FramebufferType::COLOR_STENCIL_DEPTH:
            Create();
            break;
        case FramebufferType::COLOR:
            break;
    }
}


