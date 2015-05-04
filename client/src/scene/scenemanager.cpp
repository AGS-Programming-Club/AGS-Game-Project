//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "log.hpp"
#include "scene/scenemanager.hpp"
#include "scene/scene.hpp"

bool SceneManager::debug;
std::vector<std::string> SceneManager::sceneNames;
std::vector<Scene*> SceneManager::scenes;
std::vector<std::string> SceneManager::currentSceneNames;
std::vector<Scene*> SceneManager::currentScenes;

SceneManager::SceneManager() {
	log(ERROR, "Don't instantiate this class.");
}

void SceneManager::init(bool debug) {
	if (debug) {
		log(INFO, "Initializing SceneManager");
	}
	
	SceneManager::debug = debug;
	sceneNames.clear();
	scenes.clear();
	currentScenes.clear();
	currentSceneNames.clear();
}

void SceneManager::defineScene(std::string sceneName, Scene* scene) {
	bool nameValid = true;
	
	if (sceneName == "") {
		nameValid = false;
	}
	
	int i;
	for (i = 0; nameValid == true && i < sceneNames.size(); i++) {
		if (sceneName == sceneNames[i]) {
			nameValid = false;
		}
	}
	
	if (!nameValid) {
		log(ERROR, "Invalid name.");
		return;
	}
	
	if (debug) {
		log(INFO, "Defining Scene: " + sceneName);
	}
	
	sceneNames.push_back(sceneName);
	scenes.push_back(scene);
}

void SceneManager::startScene(std::string sceneName) {
	int desiredIndex = -1;
	int i;
	for (i = 0; desiredIndex == -1 && i < sceneNames.size(); i++) {
		if (sceneName == sceneNames[i]) {
			desiredIndex = i;
		}
	}
	
	if (desiredIndex == -1) {
		log(ERROR, "No such Scene exists.");
		return;
	} else if (scenes[desiredIndex]->isRunning()) {
		log(ERROR, "Scene: " + sceneName + " is already running.");
		return;
	}
	
	if (debug) {
		log(INFO, "Starting Scene: " + sceneName);
	}
	
	scenes[desiredIndex]->setRunning(true);
	scenes[desiredIndex]->init();
	
	currentScenes.push_back(scenes[desiredIndex]);
	currentSceneNames.push_back(sceneName);
}

void SceneManager::stopScene(std::string sceneName) {
	int desiredIndex = -1;
	int i;
	for (i = 0; desiredIndex == -1 && i < sceneNames.size(); i++) {
		if (sceneName == sceneNames[i]) {
			desiredIndex = i;
		}
	}
	
	if (desiredIndex == -1) {
		log(ERROR, "No such Scene exists.");
		return;
	} else if (!scenes[desiredIndex]->isRunning()) {
		log(ERROR, "Scene: " + sceneName + " isn't running.");
		return;
	}
	
	if (debug) {
		log(INFO, "Stopping Scene: " + sceneName);
	}
	
	scenes[desiredIndex]->setRunning(false);
	scenes[desiredIndex]->dispose();
	
	desiredIndex = -1;
	for (i = 0; desiredIndex == -1 && i < currentSceneNames.size(); i++) {
		if (sceneName == currentSceneNames[i]) {
			desiredIndex = i;
		}
	}
	
	if (desiredIndex == -1) {
		log(ERROR, "This should never happen.");
	} else {
		currentScenes.erase(currentScenes.begin() + desiredIndex);
		currentSceneNames.erase(currentSceneNames.begin() + desiredIndex);
	}
}

void SceneManager::update() {
	int i;
	for (i = 0; i < currentScenes.size(); i++) {
		currentScenes[i]->update();
	}
}
