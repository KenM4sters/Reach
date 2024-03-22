#include "OpenGLContext.h"
#include "../../Helpers/Includes.h"

OpenGLContext::OpenGLContext(std::shared_ptr<Window> window) 
{ 
    uint16_t width = window->GetWindowWidth();
    uint16_t height = window->GetWindowHeight();
    std::string name = window->GetWindowName();
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    std::cout<<"INITIALIZING::GLFWWindow"<<std::endl;

    if(gl_window == nullptr) {
        std::cout << "FAILED to create GLFW window!" << std::endl;
    }

    glfwSetWindowUserPointer(gl_window, window);
    // The following functions are glfw-specific callback functions, where the 2nd parameter
    // takes in one of our application functions and fillts its parameters with the appropriate
    // data. 
    glfwSetKeyCallback(gl_window, on_key_callback);
    glfwSetCursorPosCallback(gl_window, on_mouse_move_callback);
    glfwSetFramebufferSizeCallback(gl_window, on_window_resize_callback);
    glfwMakeContextCurrent(gl_window);
    glfwSetInputMode(gl_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // In order to use modern OpenGL, we need to load a library that allows us to access the pointers
    // of the OpenGL api, of which there are many to chose from, with the two most popular being GLAD
    // and GLUT. 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "FAILED to initialize GLAD" << std::endl;
    }


    glEnable(GL_DEPTH_TEST);
}

void OpenGLContext::Init() 
{

}
void OpenGLContext::SwapBuffers() 
{
    
}