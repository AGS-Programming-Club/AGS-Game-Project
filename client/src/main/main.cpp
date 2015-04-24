#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "inputio/keybinds.hpp"
#include "log.hpp"

using namespace std;
using namespace glm;

void w() {
	render::getCameraPos()->y -= 0.001;
	render::updateCamera();
}
void a() {
	render::getCameraPos()->x += 0.001;
	render::updateCamera();
}
void s() {
	render::getCameraPos()->y += 0.001;
	render::updateCamera();
}
void d() {
	render::getCameraPos()->x -= 0.001;
	render::updateCamera();
}

void innit() {
	render::init(3, 3, 4, "AGS Programing Group", true);
	glfwSwapInterval(1);
	keybinds::init();

	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);

	keybinds::add(w, GLFW_KEY_W, KEY_DOWN);
	keybinds::add(a, GLFW_KEY_A, KEY_DOWN);
	keybinds::add(s, GLFW_KEY_S, KEY_DOWN);
	keybinds::add(d, GLFW_KEY_D, KEY_DOWN);

	//texturedTriangle::add(vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(0, 0), vec2(1, 0), vec2(1, 1), 0);
	//texturedTriangle::add(vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5), vec2(0, 0), vec2(1, 1), vec2(0, 1), 0);

	text::add(vec2(-1, 0), vec4(0.5, 1, 1, 1), 0.1, "abcd TEXT text Texty text", 0);
}

namespace fps {
	double delta;
	double lastFrame = glfwGetTime();
}

void gameLoop() {
	do {
		double time = glfwGetTime();
		fps::delta = time - fps::lastFrame;
		fps::lastFrame = time;

		render::tick();
		keybinds::poll();

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
