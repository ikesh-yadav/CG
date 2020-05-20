#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//include for uniform location caching
#include <unordered_map>

class Shader
{
private:
    unsigned int m_RendererID;
    GLuint compileShader(const GLchar*, unsigned int type) const;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    ~Shader();


    // Bind or use the current shader
    void Bind() const;
    void UnBind() const;

 

    //set uniforms using vecctors
    void SetUniform1f( const std::string& name, glm::vec1 value );
    void SetUniform2f( const std::string& name, glm::vec2 value );
	void SetUniform3f( const std::string& name, glm::vec3 value );
    void SetUniform4f( const std::string& name, glm::vec4 value );

    //set uniforms using values directly
    void SetUniform1f( const std::string& name, GLfloat value0 );
    void SetUniform2f( const std::string& name, GLfloat value0, GLfloat value1 );
    void SetUniform3f( const std::string& name, GLfloat value0, GLfloat value1, GLfloat value2 );
    void SetUniform4f( const std::string& name, GLfloat value0, GLfloat value1, GLfloat value2, GLfloat value3 );

    void SetUniform1i( const std::string& name, GLint value );
	void SetUniform2i( const std::string& name, GLint value0, GLint value1);
    void SetUniform3i( const std::string& name, GLint value0, GLint value1, GLint value2);
    void SetUniform4i( const std::string& name, GLint value0, GLint value1, GLint value2, GLint value3);




    void SetUniform4fv(const std::string& name, glm::mat4 matrix, GLboolean normalized);

    int GetUniformLocation(const std::string& name);


    int getRendererID() const { return m_RendererID; }
};

#endif