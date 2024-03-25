#pragma once
#include "RendererAPI.h"

class Renderer {
    public:
        static void Submit(const std::shared_ptr<VertexArray> VAO);
    public:
        static RendererAPI* m_rendererAPI;

};
