#version 330

in vec2 f_textureCoords;

uniform sampler2D image;

out vec4 colour;

void main() {
	colour = texture(image, f_textureCoords);
}