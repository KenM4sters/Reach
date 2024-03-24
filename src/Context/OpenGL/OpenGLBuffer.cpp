#include "OpenGLBuffer.h"

////////////////////////////////////////////////////////////////
// Vertex Buffer
////////////////////////////////////////////////////////////////
OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<Vertex>& vertices, uint32_t size)
    : m_vertices(vertices)
{
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);
}

void OpenGLVertexBuffer::Bind() const 
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void OpenGLVertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() 
{
    glDeleteBuffers(1, &m_VBO);
}


////////////////////////////////////////////////////////////////
// Index Buffer
////////////////////////////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size) 
{
    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void OpenGLIndexBuffer::Bind() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void OpenGLIndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() 
{
    glDeleteBuffers(1, &m_EBO);
}
