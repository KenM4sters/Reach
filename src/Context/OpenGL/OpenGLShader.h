#pragma once
#include "../../Renderer/Shader.h"

class OpenGLShader : public Shader {
    public:
        OpenGLShader(std::string& name, const char* vert_src, const char* frag_src, const char* geo_src = nullptr) 
            : Shader(name, vert_src, frag_src, geo_src) 
        {
            LoadFromFile(vert_src, frag_src, geo_src);
        }
        void Use() override;
        void Release() override;
        void LoadFromFile(const char* vert_src, const char* frag_src, const char* geo_src = nullptr) override;
        void Compile(const char* v_shader, const char* f_shader, const char* g_shader = nullptr) override;
        void SetFloat(std::string name, float value) override;
        void SetInt(std::string name, int value) override;
        void SetVec2f(std::string name, float x, float y) override;
        void SetVec2f(std::string name, const glm::vec2& value) override;
        void SetVec3f(std::string name, float x, float y, float z) override;
        void SetVec3f(std::string name, const glm::vec3& value) override;
        void SetVec4f(std::string name, float x, float y, float z, float w) override;
        void SetVec4f(std::string name, const glm::vec4& value) override;
        void SetMat4f(std::string name, const glm::mat4& matrix) override;
        void CheckCompilationErrors(unsigned int object, std::string type) override;
    private:
};