#version 330 core

uniform vec2 c_pos;
uniform vec2 c_scale;

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;

out vec4 f_colour;

void main() {
	f_colour = colour;
	gl_Position = vec4((position - c_pos) * c_scale, 0, 1);
}