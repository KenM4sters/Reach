#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include "../Layer.h"
#include "../SceneObject.h"

class Window;

class Interface : public Layer {
    public:
        Interface();
        ~Interface() {
            OnDetach();
        }
        void OnAttach() override;
        void OnDetach() override;
        void Begin() override; 
        void End() override; 
        
        // Function to update the interface with our scene objects.
        void AddSceneObject(std::shared_ptr<SceneObject>& sceneObj);
};