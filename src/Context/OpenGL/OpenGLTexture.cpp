#include "OpenGLTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


OpenGLTexture2D::OpenGLTexture2D(const char* path, const char* name, std::string dir)
{
    // The Up vector in OpenGL is (0.0, 1.0, 0.0), so we need to flip textures on load,
    // otherwise they'll appear upside down on our meshes.
    // Keep in mind that for some textures (such as cubeMaps), you don't want this to be
    // set to true, so having this called everytime we want a texture2D ensures that it will
    // be set back to true before we load a texture2D.
    stbi_set_flip_vertically_on_load(true);

    // For model loading, we take in a combintaion of a directory and a root file path,
    // so we'll first test if an image exists at that path first, and if it doesn't,
    // we'll just use the path.
    m_debugName = name;
    std::string filename = std::string(path);
    filename = dir + '/' + filename;

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width , &height, &nrChannels, 0);

    m_height = height;
    m_width = width;

    if(!data) {
        data = stbi_load(path, &width , &height, &nrChannels, 0);
    } 

    if(data)
    {
        GLenum format;  
        if(nrChannels == 1)
            format = GL_RED;
        else if(nrChannels == 3)
            format = GL_RGB;
        else if(nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, this->m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        this->Unbind();
        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::OpenGLTexture::Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}