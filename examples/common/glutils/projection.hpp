#pragma once
#include <array>

class Projection
{
    public:
        std::array<float, 16> createOrthoProjection(float left, float right, float top, float bottom, float near, float far) const;
};
