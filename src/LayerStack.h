#pragma once
#include "Layer.h"
#include "Core.h"

class LayerStack {
    public:
        LayerStack() 
        {
            m_layerInsert = m_layers.begin();
        }
        ~LayerStack();
        
        inline Layer* GetOverlay() const { return m_overlay;}

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        

        // Setting iterators so that we can iterate over the LayerStack object in our
        // main application loop. 
        // ----------------------------------------------------------------
        std::vector<Layer*>::iterator begin() {return m_layers.begin();}
        std::vector<Layer*>::iterator end() {return m_layers.end();}
        // ----------------------------------------------------------------
        
    private:   
        std::vector<Layer*> m_layers;
        Layer* m_overlay;
        std::vector<Layer*>::iterator m_layerInsert;
};