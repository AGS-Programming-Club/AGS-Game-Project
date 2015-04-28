#ifndef PLATFORM_H
#define PLATFORM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>

namespace platform {
	struct Data {
		float top;
		float right;
		float bottom;
		float left;
	};
	
	Data* init(float top, float right, float bottom, float left);
	void getCorner();
}

#endif