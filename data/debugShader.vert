#version 330 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 _color;
uniform mat4 cameraProjection;
out vec4 color;

void main()
{
	gl_Position = cameraProjection * vec4(vertexPosition.xy, 1.0f, 1.0f);
	color = _color;
}