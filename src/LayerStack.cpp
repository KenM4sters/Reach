#include "LayerStack.h"

LayerStack::~LayerStack() {
    for(auto layer : m_layers) {
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer) {
    m_layerInsert = m_layers.emplace(m_layerInsert, layer);
    layer->OnAttach();
}

void LayerStack::PushOverlay(Layer* layer) 
{
    m_overlay = layer;
    layer->OnAttach();
}

