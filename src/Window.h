#pragma once
#include "Core.h"
#include "Renderer/GraphicsContext.h"

/**
 * The Window class is resonsible for handling everything to do with our window, from simply
 * setting it up (using GLFW), creating a graphics context and handling user input.
*/

struct WindowProps {
    uint16_t Width = 600;
    uint16_t Height = 800;
    std::string Name;

    uint16_t MousePosX = 0;
    uint16_t MousePosY = 0;
    uint16_t PrevMousePosX = 0;
    uint16_t PrevMousePosY = 0;
    bool MouseIn = false;
};

class Window {
    public:
        Window(std::string name, uint16_t w, uint16_t h);
        void Update();

        // Getters
        inline GLFWwindow* GetNativeWindow() const {return m_window;}
        inline void SetNativeWindow(GLFWwindow* window) {m_window = window;}
        inline bool IsRunning() const {return m_isRunning;}
        inline WindowProps GetWindowProps() const {return m_windowProps;}

    private:
        GLFWwindow* m_window = nullptr;
        GraphicsContext* m_context = nullptr;
        WindowProps m_windowProps;
        bool m_isRunning = true;
};