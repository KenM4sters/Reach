#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

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
        }
        void Run(); 

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        inline static App& GetInstance() {return *s_instance;}
        inline Window& GetWindow() {return *m_window;} 
    private:
        std::shared_ptr<Window> m_window = nullptr;
        std::unique_ptr<LayerStack> m_layerStack = nullptr;
        static App* s_instance;
};