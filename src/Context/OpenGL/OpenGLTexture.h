#pragma once
#include "../../Renderer/Texture.h"

class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const char* path, const char* name, std::string dir = "");
        inline uint32_t GetWidth() override { return m_width; }
        inline uint32_t GetHeight() override { return m_height; }
        const unsigned char* GetData() const {return m_data;}
    private:
        unsigned char* m_data = nullptr;
    
};
class OpenGLCubeTexture : public CubeTexture {
    public:
        OpenGLCubeTexture(const char* path, const char* name);
        inline uint32_t GetWidth() override { return m_width; }
        inline uint32_t GetHeight() override { return m_height; }
        const unsigned char* GetData() const {return m_data;}
    private:
        unsigned char* m_data = nullptr;
    
};
