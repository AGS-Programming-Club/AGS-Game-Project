#include "inputio/keybinds.hpp"
#include "render/renderer.hpp"
#include <vector>
#include <map>
#include <GLFW/glfw3.h>
#include "inputio/textInput.hpp"
#include "log.hpp"

using namespace std;

namespace keybinds {
	struct Event {
		int keycode;
		void (*run)();
	};

	/** The vectors for the tasks */
	vector<Event> down = vector<Event>();
	vector<Event> up = vector<Event>();
	vector<Event> pressed = vector<Event>();
	vector<Event> released = vector<Event>();

	int changes[348];

	/** Adds an action to run on a key event */
	void add(void (*run)(), int keycode, EventCode event) {
		Event e = Event();
		e.keycode = keycode;
		e.run = run;

		switch(event) {
			case KEY_DOWN:
				down.push_back(e);
				break;

			case KEY_UP:
				up.push_back(e);
				break;

			case KEY_PRESSED:
				pressed.push_back(e);
				break;

			case KEY_RELEASED:
				released.push_back(e);
		}
	}

	/** Removes an action to run on a key event */
	void remove(void (*run)(), int keycode, EventCode event) {
		switch(event) {
			case KEY_DOWN:
				for(int i = 0; i < down.size(); i++) {
					if(down[i].keycode == keycode && down[i].run == run)
						down.erase(down.begin() + i);
				}
				break;

			case KEY_UP:
				for(int i = 0; i < up.size(); i++) {
					if(up[i].keycode == keycode && up[i].run == run)
						up.erase(up.begin() + i);
				}
				break;

			case KEY_PRESSED:
				for(int i = 0; i < pressed.size(); i++) {
					if(pressed[i].keycode == keycode && pressed[i].run == run)
						pressed.erase(pressed.begin() + i);
				}
				break;

			case KEY_RELEASED:
				for(int i = 0; i < released.size(); i++) {
					if(released[i].keycode == keycode && released[i].run == run)
						released.erase(released.begin() + i);
				}
				break;
		}
	}

	TextInput::TextRequest* currentRequest;
	bool consumePresses;
	int finishKeycode;

	void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		int n = 0;

		switch(action) {
			case GLFW_PRESS:
				n = 1;

				if(currentRequest == NULL) {
					if(mods && GLFW_MOD_CONTROL != 0) {
						switch(key) {
							case GLFW_KEY_A:
								currentRequest->ctrlA();
								break;
							case GLFW_KEY_C:
								currentRequest->ctrlC();
								break;
							case GLFW_KEY_V:
								currentRequest->ctrlV();
								break;
						}
					}

					if(consumePresses)
						return;
				}

				break;
			case GLFW_RELEASE:
				n = 2;
				break;
		}

		changes[key] = n;
	}

	void charCallback(GLFWwindow* window, unsigned int codepoint) {
		if(currentRequest != NULL) {
			currentRequest->addChar(codepoint);
		}
	}

	void init() {
		glfwSetKeyCallback(render::getWindow(), callback);
		glfwSetCharCallback(render::getWindow(), charCallback);
	}

	/** Polls the input devices and runs the added keybinds */
	void poll() {
		if(currentRequest != NULL) {
			bool shift = glfwGetKey(render::getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(render::getWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

			if(glfwGetKey(render::getWindow(), finishKeycode) == GLFW_PRESS) {
				currentRequest->finish();
			}

			if(glfwGetKey(render::getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
				if(shift) {
					currentRequest->leftShiftArrow();
				} else {
					currentRequest->leftArrow();
				}
			}

			if(glfwGetKey(render::getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
				if(shift) {
					currentRequest->rightShiftArrow();
				} else {
					currentRequest->rightArrow();
				}
			}

			if(consumePresses)
				return;
		}

		for(Event e : down) {
			if(glfwGetKey(render::getWindow(), e.keycode) == GLFW_PRESS)
				e.run();
		}

		for(Event e : up) {
			if(glfwGetKey(render::getWindow(), e.keycode) == GLFW_RELEASE)
				e.run();
		}

		for(Event e : pressed) {
			if(changes[e.keycode] == 1)
				e.run();
		}

		for(Event e : released) {
			if(changes[e.keycode] == 2)
				e.run();
		}

		for(int i = 0; i < 348; i++) {
			changes[i] = 0;
		}
	}

	void finishRequest() {
		currentRequest->finish();
	}

	void addTextRequest(bool overright, TextInput::TextRequest* newRequest, int finishCode, bool consume) {
		if(currentRequest != NULL) {
			if(!overright) {
				log(ERROR, "Text request already requested");
				return;
			} else {
				currentRequest->finish();
			}
		}

		consumePresses = consume;
		finishKeycode = finishCode;
		currentRequest = newRequest;
	}

	/** This will not fire events so make sure to call finish beforehand */
	void removeTextRequest() {
		currentRequest = NULL;
	}
}
