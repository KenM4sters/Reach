#pragma once
#include "../../Renderer/GraphicsContext.h"
#include "../../Window.h"


class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(std::shared_ptr<Window> window);
        void Init() override;
        void SwapBuffers() override;
    private:
        
};