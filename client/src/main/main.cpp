#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "inputio/keybinds.hpp"
#include "player/player.hpp"
#include "player/platform.hpp"
#include "log.hpp"

using namespace std;
using namespace glm;

/*void w() {
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
}*/

platform::Data* platformLocation;
bool paused = false;

void wdown() {
	player::jump();
	render::updateCamera();
}
void adown() {
	player::move(-1);
	render::updateCamera();
}
void sdown() {
}
void ddown() {
	player::move(1);
	render::updateCamera();
}

void wup() {
}
void aup() {
	player::slowDown();
	render::updateCamera();
}
void sup() {
}
void dup() {
	player::slowDown();
	render::updateCamera();
}

void pause() {
	paused = !paused;
}

void innit() {
	render::init(3, 3, 4, "AGS Programing Group", false);
	glfwSwapInterval(1);
	keybinds::init();

	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);

	keybinds::add(wup, GLFW_KEY_W, KEY_RELEASED);
	keybinds::add(aup, GLFW_KEY_A, KEY_RELEASED);
	keybinds::add(sup, GLFW_KEY_S, KEY_RELEASED);
	keybinds::add(dup, GLFW_KEY_D, KEY_RELEASED);
	keybinds::add(wdown, GLFW_KEY_W, KEY_DOWN);
	keybinds::add(adown, GLFW_KEY_A, KEY_DOWN);
	keybinds::add(sdown, GLFW_KEY_S, KEY_DOWN);
	keybinds::add(ddown, GLFW_KEY_D, KEY_DOWN);
	keybinds::add(pause, GLFW_KEY_SPACE, KEY_PRESSED);

	//texturedTriangle::add(vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(0, 0), vec2(1, 0), vec2(1, 1), 0);
	//solidTriangle::add(vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5), vec4(0.5, 1, 0.5, 1));

	//text::add(vec2(-1, 0), vec4(0.5, 1, 1, 1), 0.1, "abcd TEXT text Texty text", 0);

	//line::add(vec2(-0.5, 0), vec2(0, 1), vec4(1, 0.5, 0.5, 1));
	
	player::init();
	platformLocation = platform::init(0, 1, -0.1, -1);
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
		
		if (!paused) {
			player::update();
			player::checkCollision(platformLocation);
		}

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
