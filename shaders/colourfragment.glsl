#version 430 core

layout(location = 0) in vec4 colour;

out vec4 out_colour;

void main() {
	out_colour = colour;
}