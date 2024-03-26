#pragma once
#include "../../Renderer/VertexArray.h"
#include "OpenGLBuffer.h"

class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray(VertexBuffer* vertex_buffer, IndexBuffer* index_buffer);
        OpenGLVertexArray(VertexBuffer* vertex_buffer);
        virtual ~OpenGLVertexArray();
        virtual void Bind() const override;
        virtual void Unbind() const override;
    private:
        uint32_t m_VAO;
};