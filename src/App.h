#pragma once
#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"

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
                -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   
                -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f  
            };
            uint32_t square_indices[] = {
                0, 1, 3,   
                1, 2, 3 
            };  
            std::vector<Vertex> vertices;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 7; j++) {
                    Vertex vertex;
                    vertex.Position = {square_vertices[i*j], square_vertices[(i*j)+1], square_vertices[(i*j)+2]};
                    vertex.Normal = {square_vertices[(i*j)+3], square_vertices[(i*j)+4], square_vertices[(i*j)+5]};
                    vertex.UV = {square_vertices[(i*j)+6], square_vertices[(i*j)+7]};
                    vertices.push_back(vertex);
                }
            }

            auto vbo = VertexBuffer::Create(vertices, vertices.size()*sizeof(float));
            auto ebo = IndexBuffer::Create(square_indices, sizeof(square_indices));
            // vao = VertexArray::Create(vbo, ebo);
            uint32_t m_VAO;
            glGenVertexArrays(1, &m_VAO);
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
};