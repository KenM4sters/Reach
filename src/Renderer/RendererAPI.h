#pragma once
#include "../Core.h"
#include "VertexArray.h"


// Possible types of APIs to call from.
enum class API {
    VOID = 0,
    OPEN_GL = 1,
    VULKAN = 2
};

class RendererAPI {
    public:
        // Getter
        inline static API GetAPI() {return m_API;}
        // Setter
        inline static void SetAPI(API contextType)
        {   
            m_API = contextType;
        }
        virtual void SetClearColor(glm::vec4& color) = 0; 
        virtual void SetViewportDimensions(uint32_t width, uint32_t height) = 0; 
        virtual void ListenToEvents() = 0; 
        virtual void SwapBuffers(GLFWwindow* window) = 0; 
        virtual void Flush() = 0;
        virtual void Draw(const std::shared_ptr<VertexArray>& VAO) = 0;
        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& VAO) = 0;
    public:
        static API m_API; 
};