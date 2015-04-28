#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <glm/glm.hpp>
#include "log.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "player/platform.hpp"
#include <iostream>

using namespace std;
using namespace glm;

namespace player {
	vec2 position = vec2(0, 0);
	vec2 speed = vec2(0, 0);
	vec2 acceleration = vec2(0, 0);
	int horizontalMovementDirection = 0;
	double maxHorizontalSpeed = 0.0005;
	double maxVerticalSpeed = 0.0005;
	double jumpingAcceleration = 0.0002;
	double maxHorizontalAcceleration = 0.00001;
	double drag = 0.00001;
	solidTriangle::Data* playerGraphics;

	void init() {
		log(INFO, "Initialising player.");
		playerGraphics = solidTriangle::add(vec2(-0.15, 0), vec2(0.15, 0), vec2(0, 0.5), vec4(1, 0.5, 0.5, 1));
	}
	
	void move(int direction) {
		if (acceleration.x < 0) {
			acceleration.x = maxHorizontalAcceleration / 4;
		}
		
		if (direction > 0) {
			acceleration.x = maxHorizontalAcceleration;
			horizontalMovementDirection = 1;
		} else if (direction < 0) {
			acceleration.x = maxHorizontalAcceleration;
			horizontalMovementDirection = -1;
		}
	}
	
	void jump() {
		if (speed.y <= 0) {
			acceleration.y = 0;
			acceleration.y = jumpingAcceleration;
		}
	}
	
	void update() {
		if (acceleration.x > 0) {
			speed.x += acceleration.x;
		} else if (acceleration.x < 0) {
			speed.x += acceleration.x;
		}
		
		acceleration -= drag;
		if (acceleration.y > 0) {
			speed.y += acceleration.y;
		} else if (acceleration.y < 0) {
			speed.y += acceleration.y;
		}
		
		if (speed.x > maxHorizontalSpeed) {
			speed.x = maxHorizontalSpeed;
		} else if (speed.x < -maxHorizontalSpeed) {
			speed.x = -maxHorizontalSpeed;
		}
		
		if (speed.y > maxVerticalSpeed) {
			speed.y = maxVerticalSpeed;
		} else if (speed.y < -maxVerticalSpeed) {
			speed.y = -maxVerticalSpeed;
		}
		
		if (speed.x > 0) {
			if (horizontalMovementDirection > 0) {
				position.x += speed.x;
			} else if (horizontalMovementDirection < 0) {
				position.x -= speed.x;
			}
		}
		
		if (speed.y > 0) {
			position.y += speed.y;
		} else if (speed.y < 0) {
			position.y += speed.y;
		}
		
		solidTriangle::remove(playerGraphics);
		
		playerGraphics->a.x = position.x - 0.15;
		playerGraphics->a.y = position.y;
		playerGraphics->b.x = position.x + 0.15;
		playerGraphics->b.y = position.y;
		playerGraphics->c.x = position.x;
		playerGraphics->c.y = position.y + 0.5;
		
		playerGraphics = solidTriangle::add(vec2(position.x - 0.15, position.y), vec2(position.x + 0.15, position.y), vec2(position.x, position.y + 0.5), vec4(1, 0.5, 0.5, 1));
		
		//cout << acceleration.y << " : " << speed.y << endl;
	}
	
	void slowDown() {
		acceleration.x = -maxHorizontalAcceleration / 10;
	}
	
	bool checkCollision(platform::Data* platformData) {
		bool collision = false;
		
		if (position.y < platformData->top) {
			speed.y = platformData->top;
			position.y = 0;
			
			collision = true;
		}
		
		return collision;
	}
}