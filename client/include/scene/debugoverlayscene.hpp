#ifndef DEBUGOVERLAYSCENE_H
#define DEBUGOVERLAYSCENE_H

#include "scene/scene.hpp"
#include <string>

class DebugOverlayScene : public Scene {
private:
	
public:
	void init();
	void dispose();
	void update();

	void addLine(std::string name, int maxLength);
	void removeLine(std::string name);
	void setLine(std::string name, std::string text);
};

DebugOverlayScene* getDebugOverlay();

#endif
