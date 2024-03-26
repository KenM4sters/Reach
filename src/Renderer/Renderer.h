#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "../Mesh.h"

class Renderer {
    public:
        static void Submit(const std::shared_ptr<Mesh>& mesh);
        static void PrepareScene();
        static void CreateScene();
        static void EndScene();
    public:
        static RendererAPI* m_rendererAPI;

};
