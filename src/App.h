#pragma once
#include "Renderer/Renderer.h"
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Renderer/Framebuffer.h"
#include "Context/OpenGL/OpenGLFramebuffer.h"

/**
 * The App class is very simple and barebones - merely acts as an entry point to initiate 
 * and render our game engine. 
*/

class App {
    public:
        App(std::string app_name, uint16_t w, uint16_t h) 
        {
            s_instance = this;

            m_window = std::make_shared<Window>(app_name, w, h);
            m_layerStack = std::make_unique<LayerStack>();
            
            auto config = FramebufferConfig() = 
            {
                1, w, h
            };
            m_FBO = std::move(Framebuffer::Create(config, true));
        
        }
        void Run(); 

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        inline static App& GetInstance() {return *s_instance;}
        inline std::shared_ptr<Window>& GetWindow() {return m_window;} 
        inline const std::shared_ptr<Framebuffer>& GetFrameBuffer() const {return m_FBO;}
    private:
        std::shared_ptr<Window> m_window = nullptr;
        std::unique_ptr<LayerStack> m_layerStack = nullptr;
        std::shared_ptr<Framebuffer> m_FBO = nullptr;
        static App* s_instance;
        
};