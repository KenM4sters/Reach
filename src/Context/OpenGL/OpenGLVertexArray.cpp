#include "OpenGLVertexArray.h"

OpenGLVertexArray::OpenGLVertexArray(VertexBuffer& vertex_buffer, IndexBuffer& index_buffer) 
{
    glGenVertexArrays(1, &m_VAO);
    this->Bind();
    vertex_buffer.Bind();
    index_buffer.Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, UV)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    this->Unbind();
    vertex_buffer.Unbind();
    index_buffer.Unbind();
}

OpenGLVertexArray::~OpenGLVertexArray() 
{
    glDeleteVertexArrays(1, &m_VAO);
}

void OpenGLVertexArray::Bind() const 
{
    glBindVertexArray(m_VAO);
}

void OpenGLVertexArray::Unbind() const 
{
    glBindVertexArray(0);
}