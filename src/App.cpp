#include "App.h"

App* App::s_instance = nullptr;

void App::Run() 
{
    while(m_window->IsRunning())
    {
        m_window->PreRender();
        
        glEnable(GL_DEPTH_TEST);
        m_FBO->Bind();
        for(const auto& layer : *m_layerStack.get()) 
        {
            layer->Update();
        }

        m_layerStack->GetOverlay()->Begin(); // Overlay is the interface in our case.
        for(const auto& layer : *m_layerStack.get()) 
        {
            layer->UpdateInterface();
        }
        m_layerStack->GetOverlay()->End();
        
        m_FBO->Unbind();
        glDisable(GL_DEPTH_TEST);

        m_window->PostRender();
        ReachCore::Time::Update();
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