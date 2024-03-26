#include "Renderer.h"
#include "../Context/OpenGL/OpenGLRendererAPI.h"

// Out of class initializer for renderer api - OpenGL by default.
RendererAPI* Renderer::m_rendererAPI = new OpenGLRendererAPI();

void Renderer::Submit(const std::shared_ptr<VertexArray>& VAO, std::shared_ptr<Shader>& shader) 
{   
    VAO->Bind();
    shader->Use();
    m_rendererAPI->DrawIndexed(VAO);
    VAO->Unbind();
    shader->Release();
}

void Renderer::PrepareScene() 
{

}

void Renderer::CreateScene() 
{

}

void Renderer::EndScene() 
{

}