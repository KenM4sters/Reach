#include "App.h"

App* App::s_instance = nullptr;

void App::Run() 
{
    while(m_window->IsRunning())
    {
        m_window->PreRender();

        vao->Bind();
        m_shader->Use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // vao->Unbind();
        // glUseProgram(0);
        
        for(const auto& layer : *m_layerStack.get()) {
            layer->Update();
        }

        m_window->PostRender();

        ReachUtils::Time::Update();
    }
}

void App::PushLayer(Layer* layer) 
{
    m_layerStack->PushLayer(layer);
}
void App::PushOverlay(Layer* layer) 
{
    m_layerStack->PushOverlay(layer);
}