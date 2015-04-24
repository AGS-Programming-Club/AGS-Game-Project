#version 330 core

uniform vec2 c_pos;
uniform vec2 c_scale;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoords;

out vec2 f_textureCoords;

void main() {
	gl_Position = vec4((position - c_pos) * c_scale, 0, 1);
	f_textureCoords = textureCoords;
}