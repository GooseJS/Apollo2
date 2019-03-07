#version 330 core

in vec3 textureCoords;

out vec3 color;

uniform sampler2DArray texAtlas;

void main()
{
	color = texture(texAtlas, textureCoords).xyz;
}