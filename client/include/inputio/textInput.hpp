#ifndef TEXT_INPUT
#define TEXT_INPUT

#include <string>
#include <vector>

namespace TextInput {
	class TextRequest {
	private:
		void (*change) (TextRequest*);
		void (*finished) (TextRequest*);
		std::string text;
		int selStart;
		int selEnd;
		int pos;
		int maxLength;


	public:
		void finish();
		void addChar(unsigned int character);

		void leftArrow();
		void rightArrow();
		void leftShiftArrow();
		void rightShiftArrow();

		void ctrlA();
		void ctrlC();
		void ctrlV();

		std::string getString();

		TextRequest(bool consumePresses, void (*finished) (TextRequest*),  void (*change) (TextRequest*), int maxLength);
	};
}

#endif
