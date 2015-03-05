#pragma once
#include <cstdint>
#include <cstddef>
#include <string>

enum class VertexAttributeType{AVEC2, AVEC4};

struct VertexAttribute
{
    VertexAttribute(const std::string& name, const uint32_t floatAmount, const float* data);
    std::string mName;
    uint32_t mFloatAmount;
    const float* mData;
};
