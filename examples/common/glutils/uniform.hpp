#pragma once
#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include <gl_core_3_3.h>

enum class UniformType{NO_TYPE, FLOAT, INT, VEC2, VEC3, VEC4, MAT2X2, MAT3X3, MAT4X4, TEXTURE, TEXTURE_ARRAY};

struct Uniform
{
    Uniform();
    Uniform(const std::string& name, UniformType t, float val);
    Uniform(const std::string& name, UniformType t, int val);
    Uniform(const std::string& name, UniformType t, glm::vec2 val);
    Uniform(const std::string& name, UniformType t, glm::vec3 val);
    Uniform(const std::string& name, UniformType t, glm::vec4 val);
    Uniform(const std::string& name, UniformType t, glm::mat2x2 val);
    Uniform(const std::string& name, UniformType t, glm::mat3x3 val);
    Uniform(const std::string& name, UniformType t, glm::mat4x4 val);
    Uniform(const std::string& name, UniformType t, GLuint val);
    Uniform(const Uniform& other);

    std::string mName;
    UniformType mType;
    union
    {
        float mFloatVal;      
        int mIntVal;
        float mVec2Val[2];    
        float mVec3Val[3];    
        float mVec4Val[4];    
        float mMat2x2Val[4];  
        float mMat3x3Val[9];  
        float mMat4x4Val[16]; 
        GLuint mTextureVal;   
    };
};
