#include "buffer.hpp"
#include <fea/assert.hpp>

Buffer::Buffer(int32_t type, int32_t usage) :
    mType(type),
    mElementAmount(0),
    mUsage(usage)
{
    glGenBuffers(1, &mBufferId);

    FEA_ASSERT(mBufferId != 0, "Generated zero buffer");
}

Buffer::Buffer(const std::vector<float>& data, int32_t type, int32_t usage) :
    mType(type),
    mUsage(usage)
{
    glGenBuffers(1, &mBufferId);

    FEA_ASSERT(mBufferId != 0, "Generated zero buffer");

    setData(data);
}

Buffer::Buffer(const std::vector<uint32_t>& data, int32_t type, int32_t usage)  :
    mType(type),
    mUsage(usage)
{
    glGenBuffers(1, &mBufferId);

    FEA_ASSERT(mBufferId != 0, "Generated zero buffer");

    setData(data);
}

Buffer::Buffer(const std::vector<uint8_t>& data, int32_t type, int32_t usage)  :
    mType(type),
    mUsage(usage)
{
    glGenBuffers(1, &mBufferId);

    FEA_ASSERT(mBufferId != 0, "Generated zero buffer");

    setData(data);
}

Buffer::~Buffer()
{
    if(mBufferId)
        glDeleteBuffers(1, &mBufferId);
}

Buffer::Buffer(Buffer&& other) : 
    mBufferId(0),
    mType(0),
    mElementAmount(0)
{
    std::swap(mBufferId, other.mBufferId);
    std::swap(mType, other.mType);
    std::swap(mElementAmount, other.mElementAmount);
}

Buffer& Buffer::operator=(Buffer&& other)
{
    std::swap(mBufferId, other.mBufferId);
    std::swap(mType, other.mType);
    std::swap(mElementAmount, other.mElementAmount);

    other.mBufferId = 0;
    other.mType = 0;
    other.mElementAmount = 0;

    return *this;
}

GLuint Buffer::getId() const
{
    return mBufferId;
}

void Buffer::bind() const
{
    glBindBuffer(mType == ARRAY_BUFFER ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, mBufferId);
}

void Buffer::setData(const std::vector<float>& data)
{
    FEA_ASSERT(data.size() > 0, "Trying to generate an empty buffer!");
    mElementAmount = data.size();
    bind();
    int32_t glUsage;
    if(mUsage == STATIC)
        glUsage = GL_STATIC_DRAW;
    else if(mUsage == DYNAMIC)
        glUsage = GL_DYNAMIC_DRAW;
    glBufferData(mType == ARRAY_BUFFER ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), glUsage);
}

void Buffer::setData(const std::vector<uint32_t>& data)
{
    FEA_ASSERT(data.size() > 0, "Trying to generate an empty buffer!");
    mElementAmount = data.size();
    bind();
    int32_t glUsage;
    if(mUsage == STATIC)
        glUsage = GL_STATIC_DRAW;
    else if(mUsage == DYNAMIC)
        glUsage = GL_DYNAMIC_DRAW;
    glBufferData(mType == ARRAY_BUFFER ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), data.data(), glUsage);
}

void Buffer::setData(const std::vector<uint8_t>& data)
{
    FEA_ASSERT(data.size() > 0, "Trying to generate an empty buffer!");
    mElementAmount = data.size();
    bind();
    int32_t glUsage;
    if(mUsage == STATIC)
        glUsage = GL_STATIC_DRAW;
    else if(mUsage == DYNAMIC)
        glUsage = GL_DYNAMIC_DRAW;
    glBufferData(mType == ARRAY_BUFFER ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint8_t), data.data(), glUsage);
}

int32_t Buffer::getElementAmount() const
{
    return mElementAmount;
}
