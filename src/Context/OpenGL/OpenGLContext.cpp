#include "OpenGLContext.h"
#include "../../Core.h"

void OpenGLContext::Init() 
{   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    

}
void OpenGLContext::SwapBuffers() 
{
    glfwSwapBuffers(m_window->GetNativeWindow());
}

