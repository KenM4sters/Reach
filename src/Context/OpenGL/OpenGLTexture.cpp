#include "OpenGLTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// OpenGL Texture 2D
//================================================================
//================================================================
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
    glGenTextures(1, &m_ID); // Don't forget to generate an ID first!
    m_debugName = name;
    std::string filename = std::string(path);
    filename = dir + '/' + filename;
    std::string file_ext = std::string(path).substr(std::string(path).find_first_of("."), std::string(path).length());
    
    std::cout << filename << std::endl;


    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width , &height, &nrChannels, 0);
    m_height = height;
    m_width = width;

    if(!data) {
        data = stbi_load(path, &width , &height, &nrChannels, 0);
        std::cout << path << std::endl;
    } 

    if(data)
    {   
        GLenum format = GL_RGB;
        if(file_ext == ".hdr") format = GL_RGB16F;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_data = data;
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "ERROR::OpenGLTexture::Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}
//================================================================
//================================================================

// OpenGL Cube Texture
//================================================================
//================================================================

OpenGLCubeTexture::OpenGLCubeTexture(const char* path, const char* name)
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
    glGenTextures(1, &m_ID); // Don't forget to generate an ID first!
    m_debugName = name;
    std::string filename = std::string(path);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width , &height, &nrChannels, 0);
    m_height = height;
    m_width = width;

    if(data)
    {   
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
        for (unsigned int i = 0; i < 6; ++i)
        {
            // note that we store each face with 16 bit floating point values
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                        512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "ERROR::OpenGLTexture::Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

OpenGLCubeTexture::OpenGLCubeTexture(uint32_t width, uint32_t height) 
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

//================================================================
//================================================================
