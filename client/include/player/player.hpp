#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include "player/platform.hpp"

namespace player {
	void init();
	void move(int direction);
	void jump();
	void update();
	void slowDown();
	bool checkCollision(platform::Data*);
}

#endif