#include "../Layer.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

class InterfaceLayer : public Layer {
    public:
        InterfaceLayer();
        ~InterfaceLayer() {}
        void OnAttach() override;
        void OnDetach() override;
        void Update() override; 
    private:

};