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
#include "scene/gamescene.hpp"

using namespace glm;

void w();
void a();
void s();
void d();

texturedTriangle::Data* triangle1;
solidTriangle::Data* triangle2;
text::Data* text1;
line::Data* line1;

void GameScene::init() {
	log(INFO, "Initializing GameScene");
	
	keybinds::add(w, GLFW_KEY_W, KEY_DOWN);
	keybinds::add(a, GLFW_KEY_A, KEY_DOWN);
	keybinds::add(s, GLFW_KEY_S, KEY_DOWN);
	keybinds::add(d, GLFW_KEY_D, KEY_DOWN);
	
	GLuint image = image::loadBMP("courier");
	texture::bind(image, 0);
	
	triangle1 = texturedTriangle::add(vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(0, 0), vec2(1, 0), vec2(1, 1), 0);
	triangle2 = solidTriangle::add(vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5), vec4(0.5, 1, 0.5, 1));
	text1 = text::add(vec2(-1, 0), vec4(0.5, 1, 1, 1), 0.1, "abcd TEXT text Texty text", 0);
	line1 = line::add(vec2(-0.5, 0), vec2(0.5, 0), vec4(1, 0.5, 0.5, 1));
}

void GameScene::dispose() {
	log(INFO, "Disposing of GameScene");
	
	keybinds::remove(w, GLFW_KEY_W, KEY_DOWN);
	keybinds::remove(a, GLFW_KEY_A, KEY_DOWN);
	keybinds::remove(s, GLFW_KEY_S, KEY_DOWN);
	keybinds::remove(d, GLFW_KEY_D, KEY_DOWN);
	
	texture::bind(0, 0);
	
	texturedTriangle::remove(triangle1);
	solidTriangle::remove(triangle2);
	text::remove(text1);
	line::remove(line1);
}

void GameScene::update() {

}

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
