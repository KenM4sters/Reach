#include "Interface.h"
#include "../App.h"


InterfaceLayer::InterfaceLayer()
    : Layer("InterfaceLayer")
{
    
}
void InterfaceLayer::OnAttach() 
{
    App& app = App::GetInstance();
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(app.GetWindow()->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

void InterfaceLayer::OnDetach() 
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void InterfaceLayer::Update() 
{   
    bool bShowDemoWindow = true;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    
    ImGui::NewFrame();
    if (bShowDemoWindow)
        ImGui::ShowDemoWindow(&bShowDemoWindow);
    ImGui::Begin("ImGui Scene Editor");  
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
} 