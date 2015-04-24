#include "inputio/keybinds.hpp"
#include "render/renderer.hpp"
#include <vector>
#include <map>
#include <GLFW/glfw3.h>

using namespace std;

namespace keybinds {
	struct Event {
		int keycode;
		void (*run)();
	};

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

	void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		int n = 0;

		switch(action) {
			case GLFW_PRESS:
				n = 1;
				break;
			case GLFW_RELEASE:
				n = 2;
				break;
		}

		changes[key] = n;
	}

	void init() {
		glfwSetKeyCallback(render::getWindow(), callback);
	}

	/** Polls the input devices and runs the added keybinds */
	void poll() {
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

		for(int i = 0; i < 350; i++) {
			changes[i] = 0;
		}
	}
}
