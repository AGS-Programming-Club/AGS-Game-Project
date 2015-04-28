#include <string.h>
#include <glm/glm.hpp>
#include "log.hpp"
#include "render/renderer.hpp"
#include "render/texture.hpp"
#include "player/platform.hpp"
#include <iostream>

using namespace std;
using namespace glm;

namespace platform {
	line::Data* line1;
	line::Data* line2;
	line::Data* line3;
	line::Data* line4;
	Data* data;
	
	Data* init(float top, float right, float bottom, float left) {
		log(INFO, "Initialising platform.");
		data = new Data;
		data->top = top;
		data->right = right;
		data->bottom = bottom;
		data->left = left;
		line1 = line::add(vec2(data->left, data->top), vec2(data->right, data->top), vec4(0.25, 0.25, 0.75, 1));
		line2 = line::add(vec2(data->right, data->top), vec2(data->right, data->bottom), vec4(0.25, 0.25, 0.65, 1));
		line3 = line::add(vec2(data->right, data->bottom), vec2(data->left, data->bottom), vec4(0.25, 0.25, 0.55, 1));
		line4 = line::add(vec2(data->left, data->bottom), vec2(data->left, data->top), vec4(0.25, 0.25, 0.45, 1));
		
		return data;
	}
}