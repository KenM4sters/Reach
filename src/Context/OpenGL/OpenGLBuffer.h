#pragma once
#include "../../Renderer/Buffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
    public:
        // @param vertices Vertices data itself.
        // @param size The total size of the array in bytes.
        OpenGLVertexBuffer(std::vector<Vertex>& vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();
        virtual void Bind() const override;
        virtual void Unbind() const override;

        std::vector<Vertex> GetVertices() const { return m_vertices;}
        uint32_t GetID() const { return m_VBO;}
    private:
        uint32_t m_VBO;
        std::vector<Vertex> m_vertices;
};

class OpenGLIndexBuffer : public IndexBuffer {
    public:
        // @param indices Indices data itself.
        // @param size The total size of the array in bytes.
        OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
        virtual ~OpenGLIndexBuffer();
        virtual void Bind() const;
        virtual void Unbind() const;
        uint32_t GetID() const { return m_EBO;}
    private:
        uint32_t m_EBO;
};