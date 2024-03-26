#include "OpenGLRendererAPI.h"

// Window-specific functions
//================================================================
void OpenGLRendererAPI::SetClearColor(glm::vec4& color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::SetViewportDimensions(uint32_t width, uint32_t height) 
{
    glViewport(0, 0, width, height);
}

void OpenGLRendererAPI::ListenToEvents() 
{
    glfwPollEvents();
}

void OpenGLRendererAPI::SwapBuffers(GLFWwindow* window) 
{
    glfwSwapBuffers(window);
}

void OpenGLRendererAPI::Flush() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//================================================================

// Functions to draw graphics 
//================================================================
void OpenGLRendererAPI::Draw(const std::shared_ptr<VertexArray>& VAO) 
{
    glDrawArrays(GL_TRIANGLES, 0, VAO->GetVertexBuffer()->GetNumVertices());
}
void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& VAO) 
{
    glDrawElements(GL_TRIANGLES, VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
}
//================================================================