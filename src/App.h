#pragma once
#include "Renderer/Renderer.h"
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Context/OpenGL/OpenGLShader.h"

/**
 * The App class is very simple and barebones - merely acts as an entry point to initiate 
 * and render our game engine. 
*/

class App {
    public:
        App(std::string app_name, uint16_t w, uint16_t h) 
        {
            s_instance = this;

            m_window = std::make_shared<Window>(app_name, w, h);
            m_layerStack = std::make_unique<LayerStack>();

            std::vector<Vertex> vertices = MakeVertexFromFloat(square_vertices);
            auto vbo = VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex));
            auto ebo = IndexBuffer::Create(square_indices, sizeof(square_indices));
            vao = static_cast<std::shared_ptr<VertexArray>>(VertexArray::Create(vbo, ebo));

            std::string name = "quad_shader";
            m_shader = static_cast<std::shared_ptr<Shader>>(new OpenGLShader(name, "src/Shaders/quad.vert", "src/Shaders/quad.frag"));


        }
        void Run(); 

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        inline static App& GetInstance() {return *s_instance;}
        inline std::shared_ptr<Window>& GetWindow() {return m_window;} 
    private:
        std::shared_ptr<Window> m_window = nullptr;
        std::unique_ptr<LayerStack> m_layerStack = nullptr;
        static App* s_instance;
        std::shared_ptr<VertexArray> vao = nullptr;
        std::shared_ptr<Shader> m_shader = nullptr;
        
};