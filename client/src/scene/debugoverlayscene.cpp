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

RenderJob::LineData* fpsLine1;
RenderJob::LineData* fpsLine2;
RenderJob::LineData* fpsLine3;
RenderJob::LineData* fpsLine4;
RenderJob::TextData* fpsText;
RenderJob* debugOverlayJob;

void DebugOverlayScene::init() {
	log(INFO, "Initializing DebugOverlayScene");
	
	debugOverlayJob = render::createRenderJob(); // not really sure if this is proper usage, feel free to improve it
	
	fpsLine1 = debugOverlayJob->addLine(vec2(-1, 0.9), vec2(1, 0.9), vec4(1, 1, 1, 1));
	fpsLine2 = debugOverlayJob->addLine(vec2(-1, 0.8), vec2(1, 0.9), vec4(1, 1, 1, 1));
	fpsLine3 = debugOverlayJob->addLine(vec2(-1, 0.7), vec2(1, 0.9), vec4(1, 1, 1, 1));
	fpsLine4 = debugOverlayJob->addLine(vec2(-1, 0.6), vec2(1, 0.9), vec4(1, 1, 1, 1));
	fpsText = debugOverlayJob->addText(vec2(-1, 0.8), vec4(1, 1, 1, 1), 0.2, "FPS: ??", 0); // the scaling of the text seems a bit off, should be `vec2(-1, 0.9)` and `0.1` I think
}

void DebugOverlayScene::dispose() {
	log(INFO, "Disposing of DebugOverlayScene");
	
	debugOverlayJob->removeLine(fpsLine1);
	debugOverlayJob->removeLine(fpsLine2);
	debugOverlayJob->removeLine(fpsLine3);
	debugOverlayJob->removeLine(fpsLine4);
	debugOverlayJob->removeText(fpsText);
}

void DebugOverlayScene::update() {
	// do stuff here (I can't work out how to change the text)
}
