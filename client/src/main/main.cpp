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
#include "timing/timer.hpp"

using namespace std;
using namespace glm;

void init() {
	render::init(3, 3, 4, "AGS Programming Group", true);
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

void gameLoop() {
	do {
		timing::getFrameClock().tick();
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
