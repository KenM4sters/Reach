#include "Window.h"
#include "Renderer/Renderer.h"
#include "Context/OpenGL/OpenGLContext.h"

static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));   
}

static void on_mouse_move_callback(GLFWwindow* window, double pos_x, double pos_y) {
    auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
    WindowProps props = p_window->GetWindowProps();

    props.MousePosX = static_cast<float>(pos_x);
    props.MousePosY = static_cast<float>(pos_y);

    if(props.MouseIn) {
        props.PrevMousePosX = props.MousePosX;
        props.PrevMousePosY = props.MousePosY;
        props.MouseIn = false;
    }
    float x_offset = props.MousePosX - props.PrevMousePosX;
    float y_offset = props.PrevMousePosY - props.MousePosY ; // Inverted since up vector is (0, 1, 0)
    props.PrevMousePosX = props.MousePosX;
    props.PrevMousePosY = props.MousePosY;
}

static void on_window_resize_callback(GLFWwindow* window, int width, int height) {
    auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
}

Window::Window(std::string name, uint16_t w, uint16_t h)
{
    m_windowProps.Name = name;
    m_windowProps.Width = w;
    m_windowProps.Height = h;

    glfwInit();
    m_window = glfwCreateWindow(m_windowProps.Width, m_windowProps.Height, m_windowProps.Name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    // The following functions are glfw-specific callback functions, where the 2nd parameter
    // takes in one of our application functions and fillts its parameters with the appropriate
    // data. 
    glfwSetKeyCallback(m_window, on_key_callback);
    glfwSetCursorPosCallback(m_window, on_mouse_move_callback);
    glfwSetFramebufferSizeCallback(m_window, on_window_resize_callback);
    glfwMakeContextCurrent(m_window);
    //  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    m_context = new OpenGLContext(this); // OpenGL for now.
    m_context->Init();

}

void Window::Render() 
{
    
}

