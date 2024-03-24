#pragma once
#include "../Core.h"


class VertexBuffer {
    public:
        virtual ~VertexBuffer() {}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        static VertexBuffer* Create(std::vector<Vertex>& vertices, uint32_t size);
};

class IndexBuffer {
    public:
        virtual ~IndexBuffer() {}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        static IndexBuffer* Create(float* indices, uint32_t size);
};