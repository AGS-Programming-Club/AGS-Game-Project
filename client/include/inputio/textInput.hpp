#ifndef TEXT_INPUT
#define TEXT_INPUT

#include <string>

class TextRequest {
public:
	void finish(bool fireEvent);
	std::string getString();
};

typedef void (*FinishedCallback) (TextRequest);
typedef void (*ChangeCallback) (TextRequest);

TextRequest requestText(bool consumePresses, FinishedCallback finished, ChangeCallback change, int finishKeycode);

#endif
