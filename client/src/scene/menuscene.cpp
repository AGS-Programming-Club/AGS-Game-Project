//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "log.hpp"
#include "inputio/keybinds.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "scene/menuscene.hpp"
#include "scene/scenemanager.hpp"

using namespace glm;

void spacePressed();

RenderJob::TextData* menuText;
RenderJob* ui;

void MenuScene::init() {
	log(INFO, "Initializing MenuScene");
	
	ui = render::getUIJob();

	keybinds::add(spacePressed, GLFW_KEY_SPACE, KEY_DOWN);
	
	menuText = ui->addText(vec2(-1, 0), vec4(1, 1, 1, 1), 0.1, "Press space to continue", 0);
}

void MenuScene::dispose() {
	log(INFO, "Disposing of MenuScene");
	
	keybinds::remove(spacePressed, GLFW_KEY_SPACE, KEY_DOWN);
	
	ui->removeText(menuText);
}

void MenuScene::update() {

}

void spacePressed() {
	SceneManager::stopScene("Menu");
	SceneManager::startScene("Game");
}
