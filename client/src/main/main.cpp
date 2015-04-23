#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "log.hpp"

using namespace std;
using namespace glm;

void innit() {
	render::init(3, 3, 4, "AGS Programing Group", false);

	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);

	//texturedTriangle::add(vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(0, 0), vec2(1, 0), vec2(1, 1), 0);
	//texturedTriangle::add(vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5), vec2(0, 0), vec2(1, 1), vec2(0, 1), 0);

	text::add(vec2(-1, 0), vec4(0.5, 1, 1, 1), 0.1, "abcd TEXT text Texty text", 0);
}

void gameLoop() {
	do {
		render::tick();

		if(CHECK_GL())
			return;

	} while(render::shouldClose());
}

void cleanUp() {
	render::clean();
}

int main() {
	innit();
	gameLoop();
	cleanUp();
}
