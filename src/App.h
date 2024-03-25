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
            const std::vector<float> square_vertices = {
                0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  
                0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   
                -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   
                -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f  
            };
            uint32_t square_indices[] = {
                0, 1, 3,   
                1, 2, 3 
            };  
            std::vector<Vertex> vertices;
            for(int i = 0; i < 4; i++) {
                Vertex vertex;
                vertex.Position = {square_vertices[(i*8) + 0], square_vertices[(i*8) + 1], square_vertices[(i*8) + 2]};
                vertex.Normal = {square_vertices[(i*8) + 3], square_vertices[(i*8) + 4], square_vertices[(i*8) + 5]};
                vertex.UV = {square_vertices[(i*8) + 6], square_vertices[(i*8) + 7]};
                vertices.push_back(vertex);
            }

            auto vbo = VertexBuffer::Create(vertices, vertices.size()*sizeof(Vertex));
            auto ebo = IndexBuffer::Create(square_indices, sizeof(square_indices));
            vao = VertexArray::Create(vbo, ebo);

            std::string name = "quad_shader";
            m_shader = new OpenGLShader(name, "src/Shaders/quad.vert", "src/Shaders/quad.frag");


        }
        void Run(); 

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        inline static App& GetInstance() {return *s_instance;}
        inline Window& GetWindow() {return *m_window;} 
    private:
        std::shared_ptr<Window> m_window = nullptr;
        std::unique_ptr<LayerStack> m_layerStack = nullptr;
        static App* s_instance;
        VertexArray* vao = nullptr;
        Shader* m_shader = nullptr;
        
};