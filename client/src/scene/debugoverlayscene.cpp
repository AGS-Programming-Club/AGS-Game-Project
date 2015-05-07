//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "log.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "scene/debugoverlayscene.hpp"

using namespace glm;
using namespace std;

RenderJob::TextData* fpsText;
RenderJob* debugOverlayJob;

void DebugOverlayScene::init() {
	log(INFO, "Initializing DebugOverlayScene");
	
	debugOverlayJob = render::createRenderJob();
	fpsText = debugOverlayJob->addText(vec2(-1, 0.85), vec4(1, 1, 1, 1), 0.1, "FPS: 123", 0); // the scaling of the text seems a bit off, should be `vec2(-1, 0.9)` and `0.1` I think
}

void DebugOverlayScene::dispose() {
	log(INFO, "Disposing of DebugOverlayScene");
	
	debugOverlayJob->removeText(fpsText);
}

void DebugOverlayScene::update() {
	static int i = 0;
	i++;

	fpsText->supply->setText("FPS: " + std::to_string(i));
	debugOverlayJob->remakeTextBuffer();
}
