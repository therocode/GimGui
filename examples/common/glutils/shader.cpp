#include "shader.hpp"
#include <gimgui/assert.hpp>
#include <vector>
#include <sstream>
#include <array>
#define GLM_FORCE_RADIANS

GLSLException::GLSLException(const std::string& message) : std::runtime_error(message)
{
}

Shader::Shader() :
    mVertexShader(0),
    mFragmentShader(0),
    mProgramId(0)
{
}

Shader::Shader(Shader&& other) :
    mVertexShader(0),
    mFragmentShader(0),
    mProgramId(0)
{
    mUniformLocations = std::move(other.mUniformLocations);
    mVertexAttributeLocations = std::move(other.mVertexAttributeLocations);
    mVertexSource = std::move(other.mVertexSource);
    mFragmentSource = std::move(other.mFragmentSource);
    std::swap(mProgramId, other.mProgramId);
    std::swap(mVertexShader, other.mVertexShader);
    std::swap(mFragmentShader, other.mFragmentShader);
}

Shader& Shader::operator=(Shader&& other)
{
    mUniformLocations = std::move(other.mUniformLocations);
    mVertexAttributeLocations = std::move(other.mVertexAttributeLocations);
    mVertexSource = std::move(other.mVertexSource);
    mFragmentSource = std::move(other.mFragmentSource);
    std::swap(mProgramId, other.mProgramId);
    std::swap(mVertexShader, other.mVertexShader);
    std::swap(mFragmentShader, other.mFragmentShader);
    return *this;
}

Shader::~Shader()
{
    if(mProgramId)
    {
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);
        glDeleteProgram(mProgramId);
    }
}

void Shader::setSource(const std::string& vertexSource, const std::string& fragmentSource)
{
    mVertexSource = vertexSource;
    mFragmentSource = fragmentSource;
}

void Shader::activate() const
{
    glUseProgram(mProgramId);
}

void Shader::deactivate() const
{
    glUseProgram(0);
}

void Shader::setUniform(const std::string& name, UniformType type, const void* value) const
{
    GIM_ASSERT(mUniformLocations.count(name) != 0, "No uniform named " + name + " exists in the shader");
    switch(type)
    {
        case UniformType::FLOAT:
        {
            glUniform1f(mUniformLocations.at(name), *((float*)value));
            break;
        }
        case UniformType::INT:
        {
            glUniform1i(mUniformLocations.at(name), *((int32_t*)value));
            break;
        }
        //case UniformType::VEC2:
        //{
        //    const glm::vec2& val2 = *((glm::vec2*)value);
        //    glUniform2f(mUniformLocations.at(name), val2.x, val2.y);
        //    break;
        //}
        //case UniformType::VEC3:
        //{
        //    const glm::vec3& val3 = *((glm::vec3*)value);
        //    glUniform3f(mUniformLocations.at(name), val3.x, val3.y, val3.z);
        //    break;
        //}
        //case UniformType::VEC4:
        //{
        //    const glm::vec4& val4 = *((glm::vec4*)value);
        //    glUniform4f(mUniformLocations.at(name), val4[0], val4[1], val4[2], val4[3]);
        //    break;
        //}
        //case UniformType::MAT2X2:
        //{
        //    const glm::mat2x2 mat = *((glm::mat2x2*)value);
        //    glUniformMatrix2fv(mUniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(mat));
        //    break;
        //}
        //case UniformType::MAT3X3:
        //{
        //    const glm::mat3x3 mat = *((glm::mat3x3*)value);
        //    glUniformMatrix3fv(mUniformLocations.at(name), 1, GL_FALSE, glm::value_ptr(mat));
        //    break;
        //}
        case UniformType::MAT4X4:
        {
            const std::array<float,16> mat = *((std::array<float,16>*)value);
            glUniformMatrix4fv(mUniformLocations.at(name), 1, GL_FALSE, mat.data());
            break;
        }
        case UniformType::TEXTURE:
        {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(mUniformLocations.at(name), 0);
            glBindTexture(GL_TEXTURE_2D, *((GLuint*)value));
            break;
        }
        case UniformType::TEXTURE_ARRAY:
        {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(mUniformLocations.at(name), 0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, *((GLuint*)value));
            break;
        }
        case UniformType::NO_TYPE:
        {
            GIM_ASSERT(false, "Cannot set null uniform on shader!\n");
            break;
        }
    }
}

void Shader::setUniform(const std::string& name, UniformType type, int32_t count, const void* value) const
{
    GIM_ASSERT(mUniformLocations.count(name) != 0, "No uniform named " + name + " exists in the shader");
    switch(type)
    {
        case UniformType::FLOAT:
        {
            glUniform1fv(mUniformLocations.at(name), count, ((float*)value));
            break;
        }
        case UniformType::INT:
        {
            glUniform1iv(mUniformLocations.at(name), count, ((int*)value));
            break;
        }
        //case UniformType::VEC2:
        //{
        //    glUniform2fv(mUniformLocations.at(name), count, ((float*)value));
        //    break;
        //}
        //case UniformType::VEC3:
        //{
        //    glUniform3fv(mUniformLocations.at(name), count, ((float*)value));
        //    break;
        //}
        //case UniformType::VEC4:
        //{
        //    glUniform4fv(mUniformLocations.at(name), count, ((float*)value));
        //    break;
        //}
        //case UniformType::MAT2X2:
        //{
        //    const glm::mat2x2* mat = ((glm::mat2x2*)value);
        //    glUniformMatrix2fv(mUniformLocations.at(name), count, GL_FALSE, glm::value_ptr(*mat));
        //    break;
        //}
        //case UniformType::MAT3X3:
        //{
        //    const glm::mat3x3* mat = ((glm::mat3x3*)value);
        //    glUniformMatrix3fv(mUniformLocations.at(name), count, GL_FALSE, glm::value_ptr(*mat));
        //    break;
        //}
        case UniformType::MAT4X4:
        {
            const std::array<float,16>* mat = ((std::array<float,16>*)value);
            glUniformMatrix4fv(mUniformLocations.at(name), count, GL_FALSE, mat->data());
            break;
        }
        case UniformType::TEXTURE:
        {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(mUniformLocations.at(name), 0);
            glBindTexture(GL_TEXTURE_2D, *((GLuint*)value));
            break;
        }
        case UniformType::TEXTURE_ARRAY:
        {
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(mUniformLocations.at(name), 0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, *((GLuint*)value));
            break;
        }
        case UniformType::NO_TYPE:
        {
            GIM_ASSERT(false, "Cannot set null uniform on shader!\n");
            break;
        }
    }
}

GLint Shader::getUniform(const std::string& name) const
{
    GIM_ASSERT(mUniformLocations.count(name) != 0, "No uniform named " + name + " exists in the shader");

    return mUniformLocations.at(name);
}

void Shader::compile(const std::unordered_map<std::string, int32_t>& attributeBindings)
{
    std::string boundVertexSource = bindAttributesToSource(mVertexSource, attributeBindings);

    const char* mVertexShaderSourcePointer = boundVertexSource.data();
    const char* mFragmentShaderSourcePointer = mFragmentSource.data();

    mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVertexShader, 1, &mVertexShaderSourcePointer, NULL);
    glCompileShader(mVertexShader);

    mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFragmentShader, 1, &mFragmentShaderSourcePointer, NULL);
    glCompileShader(mFragmentShader);

    mProgramId = glCreateProgram();
    glAttachShader(mProgramId, mVertexShader);
    glAttachShader(mProgramId, mFragmentShader);
    glLinkProgram(mProgramId);

    GLint isCompiled = 0;

    glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(!isCompiled)
    {
        GLint maxLength = 0;
        glGetShaderiv(mVertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog((size_t)maxLength);
        glGetShaderInfoLog(mVertexShader, maxLength, &maxLength, &infoLog[0]);
        std::stringstream ss;
        ss << "Error! Vertex shader compilation:\n" << std::string(&infoLog[0]) << "\n";
        throw(GLSLException(ss.str()));
    }

    isCompiled = 0;
    glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(!isCompiled)
    {
        GLint maxLength = 0;
        glGetShaderiv(mFragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog((size_t)maxLength);
        glGetShaderInfoLog(mFragmentShader, maxLength, &maxLength, &infoLog[0]);
        std::stringstream ss;
        ss << "Error! Fragment shader compilation:\n" << std::string(&infoLog[0]) << "\n";
        throw(GLSLException(ss.str()));
    }

    std::stringstream vertStream(mVertexSource);

    for(std::string line; std::getline(vertStream, line);)
    {
        if(line.find("uniform") != std::string::npos)
        {
            std::string name = line.substr( line.find_first_of(" ", line.find_first_of(" ") + 1) + 1, line.find_first_of("[;") - line.find_first_of(" ", line.find_first_of(" ") + 1));

            name.resize(name.size() - 1);
            mUniformLocations.emplace(name, glGetUniformLocation(mProgramId , name.c_str()));
        }
    }
    
    std::stringstream fragStream(mFragmentSource);

    for(std::string line; std::getline(fragStream, line);)
    {
        if(line.find("uniform") != std::string::npos)
        {
            std::string name = line.substr( line.find_first_of(" ", line.find_first_of(" ") + 1) + 1, line.find_first_of("[;") - line.find_first_of(" ", line.find_first_of(" ") + 1));
            name.resize(name.size() - 1);
            mUniformLocations.emplace(name, glGetUniformLocation(mProgramId , name.c_str()));
        }
    }
}

GLuint Shader::getId() const
{
    return mProgramId;
}

std::string Shader::bindAttributesToSource(const std::string& source, const std::unordered_map<std::string, int32_t>& attributeBindings)
{
    std::string result = source;

    int32_t startPos;

    while((startPos = result.find_first_of("~", 0)) != std::string::npos)
    {
        int32_t endPos = result.find_first_of("~", startPos + 1);

        std::string attributeName = result.substr(startPos + 1, endPos - startPos - 1);

        const auto& attributeIterator = attributeBindings.find(attributeName);

        if(attributeIterator != attributeBindings.end())
        {
            result.replace(startPos, endPos - startPos + 1, std::to_string(attributeIterator->second));
        }
        else
            throw(GLSLException("Attribute binding '" + attributeName + "' has not been specified"));
    }

    return result;
}
