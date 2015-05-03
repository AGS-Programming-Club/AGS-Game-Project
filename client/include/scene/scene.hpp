#ifndef SCENE_H
#define SCENE_H

class Scene {
private:
	
	bool running;
	
public:
	/** Default constructor. Don't implement this method, use `init()` instead. */
	Scene();
	
	/** Initializes the Scene. Called by SceneManager whenever this scene is used.  */
	virtual void init() = 0;
	
	/** Cleans up and such. Called by SceneManager whenever this scene is the current scene and replaced by another. */
	virtual void dispose() = 0;
	
	/** Updates the Scene. Called by SceneManager each time it is updated. */
	virtual void update() = 0;
	
	/** Returns whether or not this scene is currently running. */
	bool isRunning();
	
	/** Changes whether or not this scene is currently considered running. */
	void setRunning(bool running);
};

#endif
