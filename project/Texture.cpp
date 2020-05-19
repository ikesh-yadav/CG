
#include "Texture.h"

TextureClass::TextureClass(GLchar* path)
{
    //use ID and load texture data
    glGenTextures(1, &m_RendererID);

    int imageWidth, imageHeight;

    unsigned char* image = SOIL_load_image(path, &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(image);
};

TextureClass::~TextureClass() {
    glDeleteTextures(1, &m_RendererID);
};

void TextureClass::UnBind() {
    glBindTexture(GL_TEXTURE_2D, 0);
};

void TextureClass::Bind(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
};