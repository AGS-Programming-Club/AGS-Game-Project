#ifndef KEYBINDS
#define KEYBINDS

/** KEY_DOWN is called whenever a key is down on a poll call
 * 	KEY_UP is called whenever a key is up on a poll call
 * 	KEY_PRESSED is called whenever a key changes from up to down between this poll and the last
 * 	KEY_RELEASED is called whenever a key changes from down to up between this poll and the last
 */
enum EventCode {KEY_DOWN, KEY_UP, KEY_PRESSED, KEY_RELEASED};

namespace keybinds {
	/** Adds an action to run on a key event
	 * @param run the action to run on keypress
	 * @param keycode the GLFW keycode to run the action on, keycodes can be found here: http://www.glfw.org/docs/latest/group__keys.html
	 * @param event the event type to run the action on @see EventCode
	 **/
	void add(void (*run)(), int keycode, EventCode event);

	/** Removes an action to run on a key event, NB you must use exactly the same paramaters as
	 * @see keybinds::add or nothing will happen
	 **/
	void remove(void (*run)(), int keycode, EventCode event);

	/** Polls the input devices and runs the added keybinds
	 **/
	void poll();

	/** Adds a callback to make KEY_PRESSED and KEY_DOWN function
	 **/
	void init();
}

#endif
