#include <string>
#include "inputio/textinput.hpp"
#include "inputio/keybinds.hpp"
#include "scene/debugoverlayscene.hpp"

using namespace std;

namespace TextInput {
	TextRequest::TextRequest(bool consumePresses, void (*finished) (TextRequest*),  void (*change) (TextRequest*), int maxLength) {
		this->finished = finished;
		this->change = change;
		text = "";
		selEnd = 0;
		selStart = 0;
		pos = 0;
		this->maxLength = maxLength;
	}

	void updateTmpDebugThings(int pos, int selEnd, int selStart) {
		getDebugOverlay()->setLine("POS", "Pos: " + std::to_string(pos));
		getDebugOverlay()->setLine("START", "End: " + std::to_string(selEnd));
		getDebugOverlay()->setLine("END", "Start: " + std::to_string(selStart));
	}

	void TextRequest::addChar(unsigned int character) {
		char c = (char) character;

		if(selEnd != selStart) {
			text = text.replace(selStart, selEnd, &c, 1);
			pos = selEnd;
		} else {
			if(text.length() + 1 >= maxLength) {
				return;
			}

			text = text.insert(pos, 1, c);
			pos++;
		}

		selEnd = pos;
		selStart = pos;
		updateTmpDebugThings(pos, selEnd, selStart);
	}

	string TextRequest::getString() {
		return text;
	}

	void TextRequest::finish() {
		if(finished != NULL)
			finished(this);

		keybinds::removeTextRequest();
	}

	void TextRequest::leftArrow() {
		if(selStart != selEnd) {
			pos = selStart;
		} else if(pos != 0) {
			pos--;
		}

		selEnd = pos;
		selStart = pos;
		updateTmpDebugThings(pos, selEnd, selStart);
	}

	void TextRequest::rightArrow() {
		if(selStart != selEnd) {
			pos = selEnd;
		} else if(pos != text.length()) {
			pos++;
		}

		selEnd = pos;
		selStart = pos;
		updateTmpDebugThings(pos, selEnd, selStart);
	}

	void TextRequest::leftShiftArrow() {
		if(selEnd != selStart) {
			if(pos == selEnd) {
				selEnd--;
				pos--;
			} else {
				if(selStart != 0) {
					selStart--;
					pos--;
				}
			}
		} else {
			if(selStart != 0) {
				selStart--;
				pos--;
			}
		}

		updateTmpDebugThings(pos, selEnd, selStart);
	}

	void TextRequest::rightShiftArrow() {
		if(selEnd != selStart) {
			if(pos == selEnd) {
				if(selEnd != text.length()) {
					selEnd++;
					pos++;
				}
			} else {
				selStart++;
				pos++;
			}
		} else {
			if(selEnd != text.length()) {
				selEnd++;
				pos++;
			}
		}

		updateTmpDebugThings(pos, selEnd, selStart);
	}

	void TextRequest::ctrlA() {
		selStart = 0;
		selEnd = text.length();
		pos = text.length();
		updateTmpDebugThings(pos, selEnd, selStart);
	}

	void TextRequest::ctrlC() {
		//TODO copy
	}

	void TextRequest::ctrlV() {
		//TODO paste
	}

	void TextRequest::backspaceKey() {
		if(selEnd == selStart) {
			if(pos == 0)
				return;

			text.erase(selStart - 1, 1);
			pos--;
			selEnd = pos;
			selStart = pos;
		} else {
			text.erase(selStart, selEnd - selStart);
			pos = selStart;
			selEnd = selStart;
		}

		updateTmpDebugThings(pos, selEnd, selStart);
	}

	void TextRequest::delKey() {
		if(selEnd == selStart) {
			if(text.length() == 0)
				return;

			if(pos == text.length()) {
				backspaceKey();
				return;
			}

			text.erase(selStart, 1);
			pos++;
			selEnd = pos;
			selStart = pos;
		} else {
			text.erase(selStart, selEnd - selStart);
			pos = selStart;
			selEnd = selStart;
		}

		updateTmpDebugThings(pos, selEnd, selStart);
	}
}
