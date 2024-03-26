#pragma once
#include "../../Renderer/Texture.h"

class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const char* path, const char* name, std::string = "");
        inline uint32_t GetWidth() override { return m_width; }
        inline uint32_t GetHeight() override { return m_height; }
    
};
