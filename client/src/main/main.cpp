#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include "log.hpp"
#include "inputio/keybinds.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "scene/scenemanager.hpp"
#include "scene/debugoverlayscene.hpp"
#include "scene/gamescene.hpp"
#include "scene/menuscene.hpp"

using namespace std;
using namespace glm;

void init() {
	render::init(3, 3, 4, "AGS Programming Group", false);
	glfwSwapInterval(1);
	keybinds::init();
	SceneManager::init(false);
	
	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);
	
	SceneManager::defineScene("DebugOverlay", new DebugOverlayScene());
	SceneManager::defineScene("Game", new GameScene());
	SceneManager::defineScene("Menu", new MenuScene());
	
	SceneManager::startScene("DebugOverlay");
	SceneManager::startScene("Menu");
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
		SceneManager::update();

		if(CHECK_GL())
			return;

	} while(render::shouldClose());
}

void cleanUp() {
	render::clean();
}

int main() {
	init();
	gameLoop();
	cleanUp();
}
