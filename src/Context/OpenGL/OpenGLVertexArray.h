#pragma once
#include "../../Renderer/VertexArray.h"

class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray(VertexBuffer& vertex_buffer, IndexBuffer& index_buffer);
        virtual ~OpenGLVertexArray();
        virtual void Bind() const;
        virtual void Unbind() const;
    private:
        uint32_t m_VAO;
};