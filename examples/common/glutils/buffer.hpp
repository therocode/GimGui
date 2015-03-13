#pragma once
#include <vector>
#include <opengl.hpp>

class Buffer
{
    public:
        enum { ARRAY_BUFFER, ELEMENT_ARRAY_BUFFER };
        enum { STATIC, DYNAMIC };
        Buffer(int32_t type, int32_t usage = STATIC);
        Buffer(const std::vector<float>& data, int32_t type, int32_t usage = STATIC);
        Buffer(const std::vector<uint32_t>& data, int32_t type, int32_t usage = STATIC);
        Buffer(const std::vector<uint8_t>& data, int32_t type, int32_t usage = STATIC);
        ~Buffer();
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
        Buffer(Buffer&& other);
        Buffer& operator=(Buffer&& other);
        GLuint getId() const;
        void bind() const;
        void setData(const std::vector<float>& data);
        void setData(const std::vector<uint32_t>& data);
        void setData(const std::vector<uint8_t>& data);
        int32_t getElementAmount() const;
    private:
        GLuint mBufferId;
        int32_t mType;
        int32_t mElementAmount;
        int32_t mUsage;
};
