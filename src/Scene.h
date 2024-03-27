#pragma once
#include "Core.h"
#include "Layer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Camera/PerspectiveCamera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"

class Scene : public Layer {
    public:
        Scene();
        ~Scene();
        void OnAttach() override;
        void OnDetach() override;
        void Update() override;
        void UpdateInterface() override;
        void HandleUserInput();
    private:
        std::shared_ptr<VertexArray> vao = nullptr;
        std::shared_ptr<Shader> m_shader = nullptr;
        std::shared_ptr<Model> m_model = nullptr;
        std::shared_ptr<PerspectiveCamera> m_camera = nullptr;
        Light* m_pointLight = nullptr;

        GLFWwindow* m_window = nullptr;

};