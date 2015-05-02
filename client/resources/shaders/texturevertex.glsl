#version 330 core

uniform mat3 matrix;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoords;

out vec2 f_textureCoords;

void main() {
	gl_Position = vec4((matrix * vec3(position, 1)).xyz, 0, 1);
	f_textureCoords = textureCoords;
}