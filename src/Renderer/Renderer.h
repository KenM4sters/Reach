#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "../Mesh.h"
#include "../Camera/PerspectiveCamera.h"

class Renderer {
    public:
        static void Submit(const std::shared_ptr<Mesh>& mesh);
        static void PrepareScene(std::shared_ptr<Mesh>& mesh, std::shared_ptr<PerspectiveCamera>* camera);
        static void CreateScene();
        static void EndScene();
    public:
        static RendererAPI* m_rendererAPI;

};
