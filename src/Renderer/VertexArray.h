#pragma once
#include "../Core.h"
#include "Buffer.h"
class VertexArray {
    public:
        virtual ~VertexArray() {}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual VertexBuffer* GetVertexBuffer() const {return m_vertexBuffer;}
        virtual IndexBuffer* GetIndexBuffer() const {return m_indexBuffer;}
        static std::shared_ptr<VertexArray> Create(VertexBuffer* vertex_buffer, IndexBuffer* index_buffer);
        static std::shared_ptr<VertexArray> Create(VertexBuffer* vertex_buffer);
    protected:
        IndexBuffer* m_indexBuffer = nullptr;
        VertexBuffer* m_vertexBuffer = nullptr;
};