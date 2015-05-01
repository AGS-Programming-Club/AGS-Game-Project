#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include "log.hpp"
#include "inputio/keybinds.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "scene/scenemanager.hpp"
#include "scene/gamescene.hpp"
#include "scene/menuscene.hpp"

using namespace std;
using namespace glm;

void init() {
	render::init(3, 3, 4, "AGS Programming Group", false);
	glfwSwapInterval(1);
	keybinds::init();
	SceneManager::init(false);
	
	// I wanted to control this in the Scene classes for better modularity but I can't really since there is no way to unload an image
	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);
	
	SceneManager::defineScene("Game", new GameScene());
	SceneManager::defineScene("Menu", new MenuScene());
	
	// comment the following as appropriate if you want to see the menu screen (just a placeholder for now)
	SceneManager::changeScene("Menu");
	//SceneManager::changeScene("Game");
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
