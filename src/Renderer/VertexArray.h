#pragma once
#include "../Core.h"
#include "Buffer.h"
class VertexArray {
    public:
        virtual ~VertexArray() {}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        static VertexArray* Create(VertexBuffer* vertex_buffer, IndexBuffer* index_buffer);
};