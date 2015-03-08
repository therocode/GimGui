#include "baseshader.hpp"

std::string BaseShader::vertexSource = R"(
#version 330

uniform mat4 projection;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texCoords;

out vec3 vertexColor;
out vec2 texCoords;

void main()
{
    gl_Position = projection * vec4(in_position, 1.0);
    vertexColor = in_color;
    texCoords = in_texCoords;
})";

std::string BaseShader::fragmentSource = R"(
#version 330
precision highp float;

uniform sampler2D texture;

in vec3 vertexColor;

in vec2 texCoords;

void main()
{
    gl_FragColor = vec4(vertexColor, 1.0) * texture2D(texture, texCoords);
})";
