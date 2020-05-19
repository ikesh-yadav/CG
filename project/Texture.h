#pragma once

#include <GL/glew.h>

#include "SOIL2/SOIL2.h"

class TextureClass{
private:
    GLuint m_RendererID;
public:
    TextureClass(GLchar* path);
    ~TextureClass();

    void UnBind();
    void Bind(unsigned int slot);
};
