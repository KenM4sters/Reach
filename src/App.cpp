#include "App.h"

App* App::s_instance = nullptr;

void App::Run() 
{
    while(m_window->IsRunning())
    {
        for(const auto& layer : *m_layerStack.get()) {
            layer->Update();
        }
        m_window->Update();
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