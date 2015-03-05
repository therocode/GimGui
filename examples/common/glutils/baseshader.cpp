#include "baseshader.hpp"

std::string BaseShader::vertexSource = R"(
#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(in_position, 1.0);
    vertexColor = in_color;
})";

std::string BaseShader::fragmentSource = R"(
#version 330
precision highp float;

in vec3 vertexColor;

void main()
{
    gl_FragColor = vec4(vertexColor, 1.0);
})";
