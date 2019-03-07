#version 330 core

layout(location = 0) in vec2 vertexPosition;
uniform mat4 cameraProjection;
uniform mat4 transformMatrix;

void main()
{
	gl_Position = cameraProjection * transformMatrix * vec4(vertexPosition.xy, 1.0f, 1.0f);
}