#pragma once
#include "../Core.h"

class Shader {
    public:
        Shader(std::string& name, const char* vert_src, const char* frag_src, const char* geo_src = nullptr)
            : m_debugName(name)
        {
        }
        ~Shader() { glDeleteProgram(m_ID); }
        virtual void Use() = 0; 
        virtual void Release() = 0; 
        virtual void LoadFromFile(const char* vert_src, const char* frag_src, const char* geo_src = nullptr) = 0;
        virtual void Compile(const char* v_shader, const char* f_shader, const char* g_shader = nullptr) = 0;
        inline const std::string GetName() const { return m_debugName;}

        // Set Shader Uniforms
        //================================================================
        virtual void SetFloat(std::string name, float value) = 0;
        virtual void SetInt(std::string name, int value) = 0;
        virtual void SetVec2f(std::string name, float x, float y) = 0;
        virtual void SetVec2f(std::string name, const glm::vec2& value) = 0;
        virtual void SetVec3f(std::string name, float x, float y, float z) = 0;
        virtual void SetVec3f(std::string name, const glm::vec3& value) = 0;
        virtual void SetVec4f(std::string name, float x, float y, float z, float w) = 0;
        virtual void SetVec4f(std::string name, const glm::vec4& value) = 0;
        virtual void SetMat4f(std::string name, const glm::mat4& matrix) = 0;
        //================================================================

        virtual void CheckCompilationErrors(unsigned int object, std::string type) = 0;
    protected:
        uint32_t m_ID;
        std::string m_debugName; // Probably won't be necessary for core functionality.
};