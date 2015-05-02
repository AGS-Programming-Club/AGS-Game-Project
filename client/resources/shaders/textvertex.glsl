#version 330 core

uniform mat3 matrix;

layout(location = 0) in vec2 v_pos;
layout(location = 1) in int letterCode;
layout(location = 2) in vec2 l_pos;
layout(location = 3) in vec4 colour;
layout(location = 4) in float size;

out vec2 f_textureCoords;
out vec4 f_colour;

void main() {
	gl_Position = vec4((matrix * vec3(v_pos * size + l_pos, 1)).xyz, 0, 1);
	f_textureCoords = (vec2(letterCode % 16, letterCode / 16) + vec2(v_pos.x, 1 - v_pos.y)) / 16.0;
	f_textureCoords.y = -f_textureCoords.y;
	f_colour = colour;
}