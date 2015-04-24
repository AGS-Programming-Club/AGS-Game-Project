#ifndef KEYBINDS
#define KEYBINDS

enum Event {KEY_DOWN, KEY_UP, KEY_PRESSED, KEY_RELEASED};

namespace keybinds {
	/** Adds an action to run on a key event */
	void add(void (*run)(), int keycode, Event event);

	/** Removes an action to run on a key event */
	void remove(void (*run)(), int keycode, Event event);

	/** Polls the input devices and runs the added keybinds */
	void poll();
}

#endif
