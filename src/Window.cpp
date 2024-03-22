#include "Window.h"
#include "Renderer/Renderer.h"

Window::Window(std::string name, uint16_t w, uint16_t h) : m_width(w), m_height(h), m_name(name) 
{
   m_window = glfwCreateWindow(m_width, m_height, name.c_str(), nullptr, nullptr);
}

void Window::Render() 
{
    
}

