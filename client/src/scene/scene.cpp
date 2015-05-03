//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "log.hpp"
#include "scene/scene.hpp"

Scene::Scene() {
	running = false;
}

bool Scene::isRunning() {
	return Scene::running;
}

void Scene::setRunning(bool running) {
	Scene::running = running;
}
