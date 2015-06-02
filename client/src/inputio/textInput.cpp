#include <string>
#include "inputio/textInput.hpp"
#include "inputio/keyBinds.hpp"

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

	void TextRequest::addChar(unsigned int character) {
		if(text.length() + 1 >= maxLength) {
			return;
		}

		text = text.append(1, character);
		pos++;
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
		if(pos != 0) {
			pos--;
		}

		selEnd = pos;
		selStart = pos;
	}

	void TextRequest::rightArrow() {
		if(pos != text.length()) {
			pos++;
		}

		selEnd = pos;
		selStart = pos;
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
		}
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
		}
	}

	void TextRequest::ctrlA() {
		selStart = 0;
		selEnd = text.length();
		pos = text.length();
	}

	void TextRequest::ctrlC() {
		//TODO copy
	}

	void TextRequest::ctrlV() {
		//TODO paste
	}
}
