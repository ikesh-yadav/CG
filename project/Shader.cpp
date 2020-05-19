#include "Shader.h"

Shader::~Shader()
{
    glDeleteProgram(this->m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram( this->m_RendererID );
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
    // Vertex Shader
    compiledShader = glCreateShader(type);
    glShaderSource(compiledShader, 1, &shaderCode, NULL);
    glCompileShader(compiledShader);

    // Print compile errors if any
    glGetShaderiv(compiledShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(compiledShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
    
    // Shader Program
    this->m_RendererID = glCreateProgram();
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

//set uniforms using vecctors

void Shader::SetUniform1f(const std::string& name, glm::vec1 value)
{
    glUniform1f(GetUniformLocation(name), value[0]);
}
void Shader::SetUniform2f(const std::string& name, glm::vec2 value)
{
    glUniform2f(GetUniformLocation(name), value[0], value[1]);
}
void Shader::SetUniform3f(const std::string& name, glm::vec3 value) {
    glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]);
}
void Shader::SetUniform4f(const std::string& name, glm::vec4 value) {
    glUniform4f(GetUniformLocation(name), value[0], value[1], value[2], value[3]);
}



//set uniforms using values directly
void Shader::SetUniform1f(const std::string& name, GLfloat value)
{
    glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetUniform2f(const std::string& name, GLfloat value0, GLfloat value1)
{
    glUniform2f(GetUniformLocation(name), value0, value1);
}
void Shader::SetUniform3f(const std::string& name,GLfloat value0, GLfloat value1, GLfloat value2) {
    glUniform3f(GetUniformLocation(name), value0, value1, value2);
}
void Shader::SetUniform4f(const std::string& name, GLfloat value0, GLfloat value1, GLfloat value2, GLfloat value3)
{
    glUniform4f(GetUniformLocation(name), value0, value1, value2, value3);
}

void Shader::SetUniform1i(const std::string& name, GLint value)
{
    glUniform1i(GetUniformLocation(name),value);
}
void Shader::SetUniform2i(const std::string& name, GLint value0, GLint value1)
{
    glUniform2i(GetUniformLocation(name), value0, value1);
}
void Shader::SetUniform3i(const std::string& name, GLint value0, GLint value1, GLint value2) {
    glUniform3i(GetUniformLocation(name), value0, value1, value2);
}
void Shader::SetUniform4i(const std::string& name, GLint value0, GLint value1, GLint value2, GLint value3)
{
    glUniform4i(GetUniformLocation(name), value0, value1, value2, value3);
}


void Shader::SetUniform4fv(const std::string& name, glm::mat4 matrix,GLboolean normalize )
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, normalize, glm::value_ptr(matrix));
}




int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        //std::cerr << "Warning: Uniform " << name << " doesn't exist"<<std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}
