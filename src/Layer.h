#pragma once
#include "Core.h"

class Layer {
    public:
        Layer(const std::string& name = "Layer")
            : m_debugName(name) 
        {

        }
        virtual ~Layer() {}
        // Some layers may or may not need a mismatch of these functions, so we'll make it 
        // optional to override them.
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void Update() {} 
    private:
        std::string m_debugName;
};