#version 330 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec3 _textureCoords;

out vec3 textureCoords;

uniform mat4 orthoProjection;
uniform mat4 translation;

void main()
{
	gl_Position = orthoProjection * translation * vec4(vertexPosition.xy, 1.0f, 1.0f);
	textureCoords = _textureCoords;
}