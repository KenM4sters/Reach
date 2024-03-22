#pragma once
#include "Helpers/Includes.h"
#include "Renderer/GraphicsContext.h"
#include "Context/OpenGL/OpenGLContext.h"

class Window {
    public:
        Window(std::string name, uint16_t w, uint16_t h);
        void Render();

        // Getters
        inline GLFWwindow* GetNativeWindow() const {return m_window;}
        inline bool IsRunning() const {return m_isRunning;}
        inline uint16_t GetWindowWidth() const {return m_width;}
        inline uint16_t GetWindowHeight() const {return m_height;}
        inline std::string GetWindowName() const {return m_name;}

    private:
        GLFWwindow* m_window = nullptr;
        GraphicsContext* m_context;

        uint16_t m_width = 600;
        uint16_t m_height = 800;
        std::string& m_name;
        bool m_isRunning = true;
};