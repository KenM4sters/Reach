#pragma once
#include "Core.h"
#include "Layer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Camera/PerspectiveCamera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"
#include "Renderer/Framebuffer.h"
#include "Skybox.h"

class Scene : public Layer {
    public:
        Scene();
        ~Scene();
        void OnAttach() override;
        void OnDetach() override;
        void Update() override;
        void UpdateInterface() override;
        void HandleUserInput();
        std::shared_ptr<PerspectiveCamera> GetCamera() { return m_camera; }
    private:
        std::shared_ptr<std::vector<std::shared_ptr<Model>>> m_models; // I really should typdef std::shared_ptr.
        std::shared_ptr<PerspectiveCamera> m_camera = nullptr;
        std::shared_ptr<Mesh> m_background = nullptr;
        std::shared_ptr<Mesh> m_convolutedBackground = nullptr;
        std::shared_ptr<Framebuffer> m_backgroundFBO = nullptr;
        std::shared_ptr<Framebuffer> m_convolutedBackgroundFBO = nullptr;
        std::shared_ptr<Skybox> m_skybox = nullptr;
        Light* m_pointLight = nullptr;
        GLFWwindow* m_window = nullptr;
};