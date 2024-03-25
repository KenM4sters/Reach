#include "App.h"

App* App::s_instance = nullptr;

void App::Run() 
{
    while(m_window->IsRunning())
    {
        m_window->PreRender();

        vao->Bind();
        m_shader->Use();
        Renderer::Submit(vao);
        vao->Unbind();
        m_shader->Release();
        
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