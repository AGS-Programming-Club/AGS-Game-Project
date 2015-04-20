#version 430 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;

layout(location = 0) out vec4 out_colour;

void main() {
	out_colour = colour;
	gl_Position = vec4(position, 0, 1);
}