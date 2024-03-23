#pragma once
#include "Helpers/Includes.h"
#include "Window.h"
#include "LayerStack.h"

/**
 * The App class is very simple and barebones - merely acts as an entry point to intiate 
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
    private:
        std::shared_ptr<Window> m_window;
        std::unique_ptr<LayerStack> m_layerStack;
        static App* s_instance;
};