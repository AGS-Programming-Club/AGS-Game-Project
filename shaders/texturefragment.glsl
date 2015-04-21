#version 330

layout(location = 0) in vec2 textureCoords;

uniform sampler2D image;

out vec4 colour;

void main() {
	colour = texture(image, textureCoords);
}