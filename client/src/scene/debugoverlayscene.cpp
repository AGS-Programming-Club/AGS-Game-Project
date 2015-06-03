//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <map>

#include "log.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "scene/debugoverlayscene.hpp"
#include "timing/timer.hpp"

using namespace glm;
using namespace std;

DebugOverlayScene* INSTANCE;

map<string, RenderJob::TextData*> lines;
int nextLine = 1;
const float LINE_DEPTH = 0.15f;

RenderJob* debugOverlayJob;

DebugOverlayScene* getDebugOverlay() {
	return INSTANCE;
}

/** Adds a line to the debug display, DO NOT use a name already taken, terrible eldrich things will occur */
void DebugOverlayScene::addLine(std::string name, int maxLength) {
	lines[name] = debugOverlayJob->addText(vec2(-1, 1 - nextLine++ * LINE_DEPTH), vec4(1, 1, 1, 1), 0.1, string(' ', maxLength), 0);
}

void DebugOverlayScene::removeLine(std::string name) {
	//TODO
}

void DebugOverlayScene::setLine(string name, string text) {
	lines[name]->supply->setText(text);
	debugOverlayJob->remakeTextBuffer();
}

void DebugOverlayScene::init() {
	INSTANCE = this;

	log(INFO, "Initializing DebugOverlayScene");
	
	debugOverlayJob = render::getUIJob();
	addLine("FPS", 10);
}

void DebugOverlayScene::dispose() {
	log(INFO, "Disposing of DebugOverlayScene");
	
	for(auto name : lines) {
		removeLine(name.first);
	}
}

void DebugOverlayScene::update() {
	setLine("FPS", "FPS: " + std::to_string(int(timing::getFrameClock().TPS())));
}
