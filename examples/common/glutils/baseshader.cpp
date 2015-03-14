#include "baseshader.hpp"

std::string BaseShader::vertexSource = R"(
#version 100

uniform mat4 projection;

//layout(location = 0) in vec3 in_position;
//layout(location = 1) in vec3 in_color;
//layout(location = 2) in vec2 in_texCoords;
attribute vec3 in_position;
attribute vec3 in_color;
attribute vec2 in_texCoords;

varying vec3 vertexColor;
varying vec2 texCoords;

void main()
{
    gl_Position = projection * vec4(in_position, 1.0);
    vertexColor = in_color;
    texCoords = in_texCoords;
})";

std::string BaseShader::fragmentSource = R"(
#version 100
precision highp float;

uniform sampler2D texture;

varying vec3 vertexColor;

varying vec2 texCoords;

void main()
{
    gl_FragColor = vec4(vertexColor, 1.0) * texture2D(texture, texCoords);
})";
