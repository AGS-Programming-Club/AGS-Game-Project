#version 330 core

in vec2 f_textureCoords;
in vec4 f_colour;

uniform sampler2D image;

out vec4 colour;

void main() {
	colour = f_colour;
	colour.a = colour.a * texture(image, f_textureCoords).r;
}