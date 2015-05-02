#version 330 core

uniform mat3 matrix;

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;

out vec4 f_colour;

void main() {
	f_colour = colour;
	gl_Position = vec4((matrix * vec3(position, 1)).xy, 0, 1);
}