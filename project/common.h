#pragma once

#include "GL/glew.h"

#include <iostream>


#define ASSERT(x) if(!(x)) __debugbreak();



static bool CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    while( GLenum err = glGetError())
    {
        std::cout<<"OpenGL error "<< err <<" , at "<< fname<<":"<< line<<" - for "<<stmt<< std::endl;
        return false;
    }
    return true;
}

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR) {
        std::cout << "clearing error" << std::endl;
    };
}

#define GL_CHECK(stmt) do { GLClearError();\
            stmt; \
            ASSERT(CheckOpenGLError(#stmt, __FILE__, __LINE__)); \
        } while (0)