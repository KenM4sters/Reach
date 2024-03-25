#pragma once
#include "RendererAPI.h"
#include "Shader.h"

class Renderer {
    public:
        static void Submit(const std::shared_ptr<VertexArray>& VAO, std::shared_ptr<Shader>& shader);
        static void PrepareScene();
        static void CreateScene();
        static void EndScene();
    public:
        static RendererAPI* m_rendererAPI;

};
