#version 330 core

layout(location = 0) uniform vec2 c_pos;
layout(location = 1) uniform vec2 c_scale;

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;

out vec4 f_colour;

void main() {
	f_colour = colour;
	gl_Position = vec4((position - c_pos) * c_scale, 0, 1);
}