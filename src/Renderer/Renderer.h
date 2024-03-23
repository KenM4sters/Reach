#pragma once

// Possible types of APIs to call from.
enum class RendererAPI {
    VOID = 0,
    OPEN_GL = 1,
    VULKAN = 2
};

class Renderer {
    public:
        // Getter
        inline static RendererAPI GetAPI() {return m_rendererAPI;}
        // Setter
        inline static void SetAPI(RendererAPI contextType)
        {   
            m_rendererAPI = contextType;
        }
        static RendererAPI m_rendererAPI;
};
