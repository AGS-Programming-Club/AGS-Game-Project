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

void innit() {
	render::init(3, 3, 4, "AGS Programing Group", false);
	glfwSwapInterval(1);
	keybinds::init();

	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);

	render::getWorldJob()->addText(vec2(-1, 0), vec4(1, 1, 1, 1), 0.1, "TEST NUMBER #2", 0);
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
