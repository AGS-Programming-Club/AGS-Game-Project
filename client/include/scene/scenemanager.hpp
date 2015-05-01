#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <vector>

#include "scene/scene.hpp"

class SceneManager {
private:

	static bool debug;
	
	static std::vector<std::string> sceneNames;
	
	static std::vector<Scene*> scenes;
	
	static std::string currentSceneName;
	
	static Scene* currentScene;
	
public:

	/** Default constructor. This class isn't meant to be instantiated, so don't. */
	SceneManager();
	
	/** Initializes the SceneManager. This method also resets the SceneManager to initial conditions.
	 * @param debug Gives potentially useful verbosity. */
	static void init(bool debug = false);
	
	/** Defines a Scene for later use.
	 * @param sceneName The name of the Scene generated, used to reference a Scene in other methods. May not be an empty string or have been defined previously. */
	static void defineScene(std::string sceneName, Scene* scene);
	
	/** Changes the current Scene.
	 * @param sceneName The name of the Scene to use. The Scene name must be previously defined. */
	static void changeScene(std::string sceneName);
	
	/** Updates the current Scene. */
	static void update();
	
};

#endif
