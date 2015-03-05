#pragma once
#include <gl_core_3_3.h>
#include "buffer.hpp"

class VAO
{
    public:
        VAO();
        ~VAO();
        VAO(const VAO&) = delete;
        VAO& operator=(const VAO&) = delete;
        GLuint getId() const;
        bool isBound() const;
        void bind();
        void unbind();
        void setVertexAttribute(GLuint index, const uint32_t amount, const float* data, GLint type = GL_FLOAT);
        void setVertexAttribute(GLuint index, const uint32_t amount, const Buffer& dataBuffer, GLint type = GL_FLOAT);
        void setInstanceAttribute(GLuint index, const uint32_t amount, const Buffer& dataBuffer, uint32_t divisor, GLint type = GL_FLOAT);
        void setVertexIntegerAttribute(GLuint index, const uint32_t amount, const Buffer& dataBuffer, GLint type);
        void setInstanceIntegerAttribute(GLuint index, const uint32_t amount, const Buffer& dataBuffer, uint32_t divisor, GLint type);
    private:
        std::vector<GLint> mEnabledVertexAttributes;
        GLuint mVAOId;
        bool mIsBound;
};
