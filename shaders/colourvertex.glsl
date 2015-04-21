#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;

out vec4 f_colour;

void main() {
	f_colour = colour;
	gl_Position = vec4(position, 0, 1);
}