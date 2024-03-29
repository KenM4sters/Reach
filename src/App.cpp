#include "App.h"

App* App::s_instance = nullptr;

void App::Run() 
{
    while(m_window->IsRunning())
    {
        m_FBO->Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer::Submit(m_FBO);
        
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