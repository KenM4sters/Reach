#pragma once
#include "../../Renderer/Texture.h"

class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const char* path, const char* name, std::string dir = "");
        OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t nrChannels);
        inline uint32_t GetWidth() override { return m_width; }
        inline uint32_t GetHeight() override { return m_height; }
        const unsigned char* GetData() const {return m_data;}
    private:
        unsigned char* m_data = nullptr;
    
};
class OpenGLCubeTexture : public CubeTexture {
    public:
        OpenGLCubeTexture(const char* path, const char* name);
        OpenGLCubeTexture(uint32_t width, uint32_t height, bool GenerateMipMaps = false);
        inline uint32_t GetWidth() override { return m_width; }
        inline uint32_t GetHeight() override { return m_height; }
        const unsigned char* GetData() const {return m_data;}
    private:
        unsigned char* m_data = nullptr;
    
};
