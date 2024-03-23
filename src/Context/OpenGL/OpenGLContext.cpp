#include "OpenGLContext.h"
#include "../../Helpers/Includes.h"

void OpenGLContext::Init() 
{   
    WindowProps props = m_window->GetWindowProps();

    GLFWwindow* gl_window = m_window->GetNativeWindow();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    std::cout<<"INITIALIZING::GLFWWindow"<<std::endl;

    if(gl_window == nullptr) {
        std::cout << "FAILED to create GLFW window!" << std::endl;
    }

    // In order to use modern OpenGL (and updated version of OpenGL with more functions, types etc...), 
    // we need to load a library that allows us to access the updated api.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "FAILED to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

}
void OpenGLContext::SwapBuffers() 
{
    glfwSwapBuffers(m_window->GetNativeWindow());
}

