#include "OpenGLFramebuffer.h"
#include "../../Renderer/VertexArray.h"
#include "OpenGLShader.h"

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferConfig& config, bool drawQuad)
    : m_config(config)
{
    Create();
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
    // glBindRenderbuffer(GL_RENDERBUFFER,0);
    // glBindTexture(GL_TEXTURE_2D, 0);
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
}

void OpenGLFramebuffer::Unbind() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



