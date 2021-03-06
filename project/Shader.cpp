#include "Shader.h"
#include "common.h"

Shader::~Shader()
{
    std::cout << "Deleting shader with id:" << this->m_RendererID << std::endl;
    glDeleteProgram(this->m_RendererID);
}

void Shader::Bind() const
{
    //std::cout<<"Rendered id to bind:"<<this->m_RendererID<<std::endl;
	GL_CHECK(glUseProgram( this->m_RendererID ));
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

GLuint Shader::compileShader(const GLchar*  shaderCode, unsigned int type  ) const
{
   GLuint compiledShader;
    GLint success;
    GLchar infoLog[512];

    GL_CHECK(compiledShader = 1);
    // Vertex Shader
    GL_CHECK(compiledShader = glCreateShader(type));

    glShaderSource(compiledShader, 1, &shaderCode, NULL);
    glCompileShader(compiledShader);

    // Print compile errors if any
    glGetShaderiv(compiledShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(compiledShader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER) std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        if (type == GL_FRAGMENT_SHADER) std::cout << "ERROR::SHADER::FAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    return compiledShader;
}

std::string GetSourceCode(const GLchar* sourcePath) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string sourceCode;
    std::ifstream shaderFile;
    // ensures ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // Open files
        shaderFile.open(sourcePath);
        std::stringstream ShaderStream;
        // Read file's buffer contents into streams
        ShaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // Convert stream into string
        sourceCode = ShaderStream.str();

        return sourceCode;
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath):m_RendererID(0)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode = GetSourceCode(vertexPath);
    std::string fragmentCode = GetSourceCode(fragmentPath);

    //convert to c strings
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();


    // 2. Compile shaders
    GLuint vertex, fragment;
    GL_CHECK(vertex = compileShader(vShaderCode, GL_VERTEX_SHADER));
    GL_CHECK(fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER));
    
    // Shader Program
    GL_CHECK(this->m_RendererID = glCreateProgram());

    std::cout << "created a progrma with id " << this->m_RendererID << std::endl;
    glAttachShader(this->m_RendererID, vertex);
    glAttachShader(this->m_RendererID, fragment);
    glLinkProgram(this->m_RendererID);


    // Print linking errors if any
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(this->m_RendererID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->m_RendererID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

//set uniforms

// all commented parts use old method but requires the progrma to be bound

void Shader::SetUniform1f(const std::string& name, glm::vec1 value)
{
    glProgramUniform1f(m_RendererID, GetUniformLocation(name), value[0]);
    //glUniform1f(GetUniformLocation(name), value[0]);
}
void Shader::SetUniform2f(const std::string& name, glm::vec2 value)
{
    glProgramUniform2f(m_RendererID, GetUniformLocation(name), value[0], value[1]);
    //glUniform2f(GetUniformLocation(name), value[0], value[1]);
}
void Shader::SetUniform3f(const std::string& name, glm::vec3 value) {
    glProgramUniform3f(m_RendererID, GetUniformLocation(name), value[0], value[1], value[2]);
    //glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]);
}
void Shader::SetUniform4f(const std::string& name, glm::vec4 value) {
    glProgramUniform4f(m_RendererID, GetUniformLocation(name), value[0], value[1], value[2], value[3]);
    //glUniform4f(GetUniformLocation(name), value[0], value[1], value[2], value[3]);
}



//set uniforms using values directly
void Shader::SetUniform1f(const std::string& name, GLfloat value)
{
    glProgramUniform1f(m_RendererID, GetUniformLocation(name), value);
    //glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetUniform2f(const std::string& name, GLfloat value0, GLfloat value1)
{
    glProgramUniform2f(m_RendererID, GetUniformLocation(name), value0, value1);
    //glUniform2f(GetUniformLocation(name), value0, value1);  
}
void Shader::SetUniform3f(const std::string& name,GLfloat value0, GLfloat value1, GLfloat value2) {
    //glUniform3f(GetUniformLocation(name), value0, value1, value2);    
    glProgramUniform3f(m_RendererID, GetUniformLocation(name), value0, value1, value2);

}
void Shader::SetUniform4f(const std::string& name, GLfloat value0, GLfloat value1, GLfloat value2, GLfloat value3)
{
    glProgramUniform4f(m_RendererID, GetUniformLocation(name), value0, value1, value2, value3);
    //glUniform4f(GetUniformLocation(name), value0, value1, value2, value3);
}

void Shader::SetUniform1i(const std::string& name, GLint value)
{
    glProgramUniform1i(m_RendererID, GetUniformLocation(name),value);
    //glUniform1i(GetUniformLocation(name),value);
}
void Shader::SetUniform2i(const std::string& name, GLint value0, GLint value1)
{
    glProgramUniform2i(m_RendererID, GetUniformLocation(name), value0, value1);
    //glUniform2i(GetUniformLocation(name), value0, value1);
}
void Shader::SetUniform3i(const std::string& name, GLint value0, GLint value1, GLint value2) {
    glProgramUniform3i(m_RendererID, GetUniformLocation(name), value0, value1, value2);
    //glUniform3i(GetUniformLocation(name), value0, value1, value2);
}
void Shader::SetUniform4i(const std::string& name, GLint value0, GLint value1, GLint value2, GLint value3)
{
    glProgramUniform4i(m_RendererID, GetUniformLocation(name), value0, value1, value2, value3);
    //glUniform4i(GetUniformLocation(name), value0, value1, value2, value3);
}


void Shader::SetUniform4fv(const std::string& name, glm::mat4 matrix,GLboolean normalize )
{
    glProgramUniformMatrix4fv(m_RendererID, GetUniformLocation(name), 1, normalize, glm::value_ptr(matrix));
    //glUniformMatrix4fv(GetUniformLocation(name), 1, normalize, glm::value_ptr(matrix));
}




int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform " << name << " doesn't exist"<<" for "<<m_RendererID<<std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}
