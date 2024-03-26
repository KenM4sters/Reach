#pragma once
#include "../Core.h"


class VertexBuffer {
    public:
        virtual ~VertexBuffer() {}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetNumVertices() const = 0;

        // Creates an IndexBuffer from an vertices array and a known size of that index array.
        // @param vertices Array of vertices itself.
        // @param size Size of the array in BYTES.
        static VertexBuffer* Create(std::vector<Vertex>& vertices, uint32_t size);
    protected:
        uint32_t m_numVertices;
};

class IndexBuffer {
    public:
        virtual ~IndexBuffer() {}
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;

        // Creates an IndexBuffer from an indices array and a known size of that index array.
        // @param indices Array of indices itself.
        // @param size Size of the array in BYTES.
        static IndexBuffer* Create(uint32_t* indices, uint32_t size);
    protected:
        uint32_t m_count;
};