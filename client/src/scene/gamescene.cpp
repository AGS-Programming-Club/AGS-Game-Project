//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "log.hpp"
#include "inputio/keybinds.hpp"
#include "inputio/textInput.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "scene/gamescene.hpp"

using namespace glm;

void w();
void a();
void s();
void d();

RenderJob::TexturedTriangleData* triangle1;
RenderJob::SolidTriangleData* triangle2;
RenderJob::TextData* text1;
RenderJob::LineData* line1;
RenderJob* job;
TextInput::TextRequest* request;


void GameScene::init() {
	log(INFO, "Initializing GameScene");
	
	keybinds::add(w, GLFW_KEY_W, KEY_DOWN);
	keybinds::add(a, GLFW_KEY_A, KEY_DOWN);
	keybinds::add(s, GLFW_KEY_S, KEY_DOWN);
	keybinds::add(d, GLFW_KEY_D, KEY_DOWN);
	
	request = new TextInput::TextRequest(true, NULL,  NULL, 20);
	keybinds::addTextRequest(false, request, GLFW_KEY_ENTER, true);

	job = render::getWorldJob();

	triangle1 = job->addTexturedTriangle(vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(0, 0), vec2(1, 0), vec2(1, 1), 0);
	triangle2 = job->addSolidTriangle(vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5), vec4(0.5, 1, 0.5, 1));
	text1 = job->addText(vec2(-1, 0), vec4(0.5, 1, 1, 1), 0.1, "                    ", 0);
	line1 = job->addLine(vec2(-0.5, 0), vec2(0.5, 0), vec4(1, 0.5, 0.5, 1));
}

void GameScene::dispose() {
	log(INFO, "Disposing of GameScene");
	
	keybinds::remove(w, GLFW_KEY_W, KEY_DOWN);
	keybinds::remove(a, GLFW_KEY_A, KEY_DOWN);
	keybinds::remove(s, GLFW_KEY_S, KEY_DOWN);
	keybinds::remove(d, GLFW_KEY_D, KEY_DOWN);
	
	job->removeTexturedTriangle(triangle1);
	job->removeSolidTriangle(triangle2);
	job->removeText(text1);
	job->removeLine(line1);
}

void GameScene::update() {
	text1->supply->setText(request->getString());
}

void w() {
	job->getCameraMatrix()[2].y += 0.001;
}

void a() {
	job->getCameraMatrix()[2].x -= 0.001;
}

void s() {
	job->getCameraMatrix()[2].y -= 0.001;
}

void d() {
	job->getCameraMatrix()[2].x += 0.001;
}
