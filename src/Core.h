#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdlib.h>
#include <vector>

/**
 * Static Time utility function used to access the elapsed time in miliseconds    
*/
namespace ReachUtils 
{
    class Time {
        public:
            // Returns the time since the start of the GLFW window in seconds.
            const inline static float GetElapsedTime() 
            {
                return static_cast<float>(glfwGetTime());
            }
            // Returns the time between frames in seconds.
            const inline static float& GetDeltaTime() 
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
