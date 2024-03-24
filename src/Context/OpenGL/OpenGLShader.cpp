#include "../../Core.h"
#include "OpenGLShader.h"
#include <fstream>
#include <sstream>

void OpenGLShader::Use() 
{
    glUseProgram(m_ID);
}

void OpenGLShader::LoadFromFile(const char* vert_src, const char* frag_src, const char* geo_src) 
{
        // 1. retrieve the vertex/fragment source code from filePath
    std::string v_code;
    std::string f_code;
    std::string g_code;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vert_src);
        std::ifstream fragmentShaderFile(frag_src);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        v_code = vShaderStream.str();
        f_code = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (geo_src != nullptr)
        {
            std::ifstream geometryShaderFile(geo_src);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            g_code = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *v_shader = v_code.c_str();
    const char *f_shader = f_code.c_str();
    const char *g_shader = g_code.c_str();

    // Compilation of shader source code into OpenGL "Shader Programs".
    uint32_t s_vertex, s_fragment, s_geometry;

    s_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s_vertex, 1, &v_shader, NULL);
    glCompileShader(s_vertex);
    CheckCompilationErrors(s_vertex, "VERTEX");

    s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s_fragment, 1, &f_shader, NULL);
    glCompileShader(s_fragment);
    CheckCompilationErrors(s_fragment, "FRAGMENT");

    // If the geometry shader source code is given then compile the geometry shader.
    if (g_shader != nullptr)
    {
        s_geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(s_geometry, 1, &g_shader, NULL);
        glCompileShader(s_geometry);
        CheckCompilationErrors(s_geometry, "GEOMETRY");
    }

    this->m_ID = glCreateProgram();
    glAttachShader(this->m_ID, s_vertex);
    glAttachShader(this->m_ID, s_fragment);
    if (g_shader != nullptr)
        glAttachShader(this->m_ID, s_geometry);
    glLinkProgram(this->m_ID);
    CheckCompilationErrors(this->m_ID, "PROGRAM");
    // All shaders can be deleted now, since we've already linked them to our shader program.
    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
    if (g_shader != nullptr)
        glDeleteShader(s_geometry);
}

void OpenGLShader::SetFloat(std::string name, float value)
{
    this->Use();
    glUniform1f(glGetUniformLocation(this->m_ID, name.c_str()), value);
}

void OpenGLShader::SetInt(std::string name, int value)
{
    this->Use();
    glUniform1i(glGetUniformLocation(this->m_ID, name.c_str()), value);
}

void OpenGLShader::SetVec2f(std::string name, float x, float y)
{
    this->Use();
    glUniform2f(glGetUniformLocation(this->m_ID, name.c_str()), x, y);
}

void OpenGLShader::SetVec2f(std::string name, const glm::vec2 &value)
{
    this->Use();
    glUniform2f(glGetUniformLocation(this->m_ID, name.c_str()), value.x, value.y);
}

void OpenGLShader::SetVec3f(std::string name, float x, float y, float z)
{
    this->Use();
    glUniform3f(glGetUniformLocation(this->m_ID, name.c_str()), x, y, z);
}

void OpenGLShader::SetVec3f(std::string name, const glm::vec3 &value)
{
    this->Use();
    glUniform3f(glGetUniformLocation(this->m_ID, name.c_str()), value.x, value.y, value.z);
}

void OpenGLShader::SetVec4f(std::string name, float x, float y, float z, float w)
{
    this->Use();
    glUniform4f(glGetUniformLocation(this->m_ID, name.c_str()), x, y, z, w);
}

void OpenGLShader::SetVec4f(std::string name, const glm::vec4 &value)
{
    this->Use();
    glUniform4f(glGetUniformLocation(this->m_ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetMat4f(std::string name, const glm::mat4 &matrix)
{
    this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, false, glm::value_ptr(matrix));
}

void OpenGLShader::CheckCompilationErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
