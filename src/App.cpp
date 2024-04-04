#include "App.h"

App* App::s_instance = nullptr;

void App::Run() 
{
    while(m_window->IsRunning())
    {
        m_FBO->Bind(); // Binds our FBO and disables depth testing (since we'll only be rendering a 2D quad).
        m_window->PreRender();

        // Loops through each layer (Only the Scene for now).
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

        m_window->PreRender(); 
        Renderer::Submit(m_FBO); // Render the quad.
        
        m_window->PostRender();
        ReachCore::Time::Update(); // Useful for getting the time between frames.
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