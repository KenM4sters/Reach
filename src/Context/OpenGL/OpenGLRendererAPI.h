#pragma once
#include "OpenGLVertexArray.h"
#include "../../Renderer/VertexArray.h"
#include "../../Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI {
    public:
        // Functions to handle window-specific events
        void SetClearColor(glm::vec4& color) override;
        void SetViewportDimensions(uint32_t width, uint32_t height) override;
        void ListenToEvents() override;
        void SwapBuffers(GLFWwindow* window) override;
        void Flush() override;
        
        // Functions to draw graphics
        void Draw(const std::shared_ptr<VertexArray>& VAO) override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& VAO) override;
};