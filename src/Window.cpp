#include "Window.h"
#include "Renderer/Renderer.h"
#include "Context/OpenGL/OpenGLContext.h"
#include "Renderer/Buffer.h"

static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
    switch(key) 
    {
        case GLFW_KEY_ESCAPE:
            if(action == GLFW_PRESS) p_window->SetStatus(false);
            break;
        case GLFW_KEY_SPACE:
            if(action == GLFW_PRESS) std::cout << "SPACE_BAR pressed!" << std::endl;
            break;
        case GLFW_KEY_W:
            if(action == GLFW_PRESS) std::cout << "W key pressed!" << std::endl;
            break;
        case GLFW_KEY_A:
            if(action == GLFW_PRESS) std::cout << "A key pressed!" << std::endl;
            break;
        case GLFW_KEY_S:
            if(action == GLFW_PRESS) std::cout << "S key pressed!" << std::endl;
            break;
        case GLFW_KEY_D:
            if(action == GLFW_PRESS) std::cout << "D key pressed!" << std::endl;
            break;
    } 
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

static void on_window_close_callback(GLFWwindow* window) 
{
    auto p_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
    p_window->SetStatus(false);
}

Window::Window(std::string name, uint16_t w, uint16_t h)
{
    m_windowProps.Name = name;
    m_windowProps.Width = w;
    m_windowProps.Height = h;

    glfwInit();
    switch(Renderer::m_rendererAPI->GetAPI()) 
    {
        case API::VOID:
            throw std::runtime_error("ERROR::VertexBuffer::Create() - RendererAPI is currently set to VOID!");
            break;
        case API::OPEN_GL:
            m_context = new OpenGLContext(this); // OpenGL for now.
            break;
        case API::VULKAN:
            throw std::runtime_error("Error::VertexBuffer::Create() - RendererAPI::Vulkan is currently unavailabe.");
            break;
    }
    m_context->Init();  

    m_window = glfwCreateWindow(m_windowProps.Width, m_windowProps.Height, m_windowProps.Name.c_str(), nullptr, nullptr);
    
    std::cout<<"INITIALIZING::GLFWWindow"<<std::endl;
    if(m_window == nullptr) {
        std::cout << "FAILED to create GLFW window!" << std::endl;
    }


    // The following functions are glfw-specific callback functions, where the 2nd parameter
    // takes in one of our application functions and fillts its parameters with the appropriate
    // data. 
    //----------------------------------------------------------------
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, on_key_callback);
    glfwSetCursorPosCallback(m_window, on_mouse_move_callback);
    glfwSetFramebufferSizeCallback(m_window, on_window_resize_callback);
    glfwMakeContextCurrent(m_window);
    glfwSetWindowCloseCallback(m_window, on_window_close_callback);
    //  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //----------------------------------------------------------------

    // In order to use modern OpenGL (and updated version of OpenGL with more functions, types etc...), 
    // we need to load a library that allows us to access the updated api.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "FAILED to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
}

void Window::PreRender() 
{
    glm::vec4 color(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::m_rendererAPI->SetClearColor(color);
    Renderer::m_rendererAPI->Flush();
}
void Window::PostRender() 
{
    Renderer::m_rendererAPI->ListenToEvents();
    Renderer::m_rendererAPI->SwapBuffers(m_window);
}

inline void HandleResize(int w, int h) 
{
    Renderer::m_rendererAPI->SetViewportDimensions(w, h);
}

