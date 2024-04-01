#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "../Mesh.h"
#include "../Camera/PerspectiveCamera.h"
#include "../Light.h"
#include "../Model.h"
#include "Framebuffer.h"

class Renderer {
    public:
        static void Submit(const std::shared_ptr<Mesh>& mesh);
        static void Submit(const std::shared_ptr<Model>& model);
        static void Submit(const std::shared_ptr<Framebuffer>& fbo);
        static void PrepareScene(std::shared_ptr<std::vector<std::shared_ptr<Model>>>& models, std::shared_ptr<PerspectiveCamera>* camera, Light* light);
        static void PrepareBackground(std::shared_ptr<Mesh> mesh, std::shared_ptr<Framebuffer> FBO, std::shared_ptr<Shader> eqToCubeShader);
        static void CreateBackground(std::shared_ptr<Mesh> mesh, std::shared_ptr<PerspectiveCamera> camera);
        // May end up seperating the scene render into 3 parts.
        static void CreateScene(); 
        static void EndScene();
    public:
        static RendererAPI* m_rendererAPI;

};
