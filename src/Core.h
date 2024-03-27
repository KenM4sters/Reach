#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <map>

/**
 * Example containers for rendering a square 
*/ 
extern std::vector<float> square_vertices;
extern std::vector<float> cube_vertices;
// extern uint32_t cube_indices[24]; 
extern uint32_t square_indices[6]; 

/**
 * Vertex Struct to hold important data for every vertex of model.
 * @param Position XYZ coords in 3D space.
 * @param Normal XYZ coords of a normalised vector perpendicular to its position.
 * @param UV 2 Dimensional coordiantes that correspond to specific fragment on a texture. 
*/
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
};

// Helper function to convert a vector of floats to a vector of Vertex structs, which is 
// what the engine uses to render primitves to the screen.
std::vector<Vertex> MakeVertexFromFloat(std::vector<float> f_vertices);

/**
 * Static Time utility function used to access the elapsed time in miliseconds    
*/
namespace ReachCore 
{
    class Time {
        public:
            // Returns the time since the start of the GLFW window in seconds.
            inline static float GetElapsedTime() 
            {
                return static_cast<float>(glfwGetTime());
            }
            // Returns the time between frames in seconds.
            inline static float& GetDeltaTime() 
            {
                return m_deltaTime;
            }
            static void Update() 
            {
                m_currentFrame = GetElapsedTime();
                m_deltaTime = m_currentFrame - m_previousFrame;
                m_previousFrame = m_currentFrame; 
            }
        private:
            Time() {}
            ~Time() {}
            
            static float m_currentFrame;
            static float m_previousFrame;
            static float m_deltaTime;
    };
}
