#pragma once
#include "../../Window.h"
#include "../../Renderer/GraphicsContext.h"


class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(Window* window) : m_window(window) {}
        void Init() override;

    private:
        Window* m_window = nullptr;
        
};