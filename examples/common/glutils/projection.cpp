#include <glutils/projection.hpp>

std::array<float, 16> Projection::createOrthoProjection(float left, float right, float top, float bottom, float nearr, float farr) const
{
    float r_l = right - left;
    float t_b = top - bottom;
    float f_n = farr - nearr;
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(farr + nearr) / (farr - nearr);

    std::array<float, 16> matrix;

    matrix[0] = 2.0f / r_l;
    matrix[4] = 0.0f;
    matrix[8] = 0.0f;
    matrix[12] = tx;

    matrix[1] = 0.0f;
    matrix[5] = 2.0f / t_b;
    matrix[9] = 0.0f;
    matrix[13] = ty;

    matrix[2] = 0.0f;
    matrix[6] = 0.0f;
    matrix[10] = 2.0f / f_n;
    matrix[14] = tz;

    matrix[3] = 0.0f;
    matrix[7] = 0.0f;
    matrix[11] = 0.0f;
    matrix[15] = 1.0f;

    return matrix;
}
