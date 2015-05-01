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
std::string SceneManager::currentSceneName;
Scene* SceneManager::currentScene;

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
	currentSceneName = "";
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

void SceneManager::changeScene(std::string sceneName) {
	if (debug) {
		if (currentSceneName == "") {
			log(INFO, "Changing Scene to: " + sceneName);
		} else {
			log(INFO, "Changing Scene from: " + currentSceneName + " to: " + sceneName);
		}
	}
	
	if (currentSceneName == sceneName) {
		log(ERROR, sceneName + " is already the currently set Scene.");
		return;
	}
	
	if (currentSceneName != "" && currentScene != NULL) {
		currentScene->dispose();
		if (debug) {
			log(INFO, "Disposed old Scene: " + currentSceneName);
		}
	}
	
	int desiredIndex = -1;
	int i;
	for (i = 0; desiredIndex == -1 && i < sceneNames.size(); i++) {
		if (sceneName == sceneNames[i]) {
			desiredIndex = i;
		}
	}
	
	if (desiredIndex != -1) {
		currentSceneName = sceneName;
		currentScene = scenes[desiredIndex];
		currentScene->init();
	} else {
		log(ERROR, "Invalid `sceneName` passed to `SceneManager::changeScene`.");
		return;
	}
}

void SceneManager::update() {
	if (currentSceneName != "" && currentScene != NULL) {
		currentScene->update();
	}
}
