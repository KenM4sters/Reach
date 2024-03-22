#pragma once
#include "Helpers/Includes.h"
#include "Window.h"

class App {
    public:
        App(std::string app_name, uint16_t w, uint16_t h) 
        {
            m_window = std::make_shared<Window>(app_name, w, h);
            
        }
        void Run();
    private:
        std::shared_ptr<Window> m_window;
};