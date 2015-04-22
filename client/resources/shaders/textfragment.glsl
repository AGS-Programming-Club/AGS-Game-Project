#version 330 core

layout(location = 0) in vec2 textureCoords;

layout(location = 0) uniform sampler2D image;

out vec4 colour;

void main() {
	colour = texture(image, textureCoords);
}