#pragma once
#include "Core.h"

class Layer {
    public:
        Layer(const std::string& name = "Layer")
            : m_debugName(name) 
        {

        }
        virtual ~Layer() 
        {
            OnDetach();
        }
        // Some layers may or may not need a mismatch of these functions, so we'll make it 
        // optional to override them.
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void Update() {}
        virtual void UpdateInterface() {}
        virtual void Begin() {}
        virtual void End() {}

        std::string GetName() const {
            return m_debugName;
        }
    private:
        std::string m_debugName;
};