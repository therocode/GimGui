#pragma once
#include "uniform.hpp"
#include "vertexattribute.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

class GLSLException : public std::runtime_error 
{
    public:
        GLSLException(const std::string& message);
};

class Shader
{
    public:

        Shader();
        Shader(const Shader& other) = delete;
        Shader& operator=(const Shader& other) = delete;
        Shader(Shader&& other);
        Shader& operator=(Shader&& other);
        ~Shader();
        void setSource(const std::string& vertexSource, const std::string& fragmentSource);
        void activate() const;
        void deactivate() const;
        void setUniform(const std::string& name, UniformType type, const void* value) const;
        void setUniform(const std::string& name, UniformType type, int32_t count, const void* value) const;
        GLint getUniform(const std::string& name) const;
        void compile(const std::unordered_map<std::string, int32_t>& attributeBindings = std::unordered_map<std::string, int32_t>());
        GLuint getId() const;
    private:
        std::string bindAttributesToSource(const std::string& source, const std::unordered_map<std::string, int32_t>& attributeBindings);
        GLuint mProgramId;
        std::unordered_map<std::string, GLint> mUniformLocations;
        std::unordered_map<std::string, GLint> mVertexAttributeLocations;
        std::string mVertexSource;
        std::string mFragmentSource;
        GLuint mVertexShader;
        GLuint mFragmentShader;
};
