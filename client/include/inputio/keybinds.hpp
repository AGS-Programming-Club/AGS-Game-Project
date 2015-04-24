#ifndef KEYBINDS
#define KEYBINDS

enum EventCode {KEY_DOWN, KEY_UP, KEY_PRESSED, KEY_RELEASED};

namespace keybinds {
	/** Adds an action to run on a key event */
	void add(void (*run)(), int keycode, EventCode event);

	/** Removes an action to run on a key event */
	void remove(void (*run)(), int keycode, EventCode event);

	/** Polls the input devices and runs the added keybinds */
	void poll();

	void init();
}

#endif
