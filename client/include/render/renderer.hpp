#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <string>
#include <cstring>
#include <glm/glm.hpp>

#define SFILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define CHECK_GL() render::checkGL("GL Error " + std::string(SFILE) + "." + LINE_STRING + ": ")

namespace render {
	void init(int major, int minor, int samples, std::string title, bool fullscreen);
	void tick();
	bool shouldClose();
	void clean();
	bool checkGL(std::string message);
}

namespace texture {
	//binds an image to a specified texture unit that can be used in texturedTriangle::add
	void bind(GLuint image, int unit);
}

namespace solidTriangle {
	struct Data {
		Data* previous;
		Data* next;
		glm::vec2 a;
		glm::vec2 b;
		glm::vec2 c;
		glm::vec4 colour;
	};

	Data* add(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec4 colour);
	void remove(Data* triangle);
}

namespace texturedTriangle {
	struct Data {
		Data* previous;
		Data* next;

		glm::vec2 a;
		glm::vec2 b;
		glm::vec2 c;

		glm::vec2 p;
		glm::vec2 q;
		glm::vec2 r;

		int texture; //this may be removed later
	};

	Data* add(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p, glm::vec2 q, glm::vec2 r, int texture);
	void remove(Data* triangle);
}

namespace line {
	struct Data {
		Data* previous;
		Data* next;

		glm::vec2 a;
		glm::vec2 b;

		glm::vec4 colour;
	};

	Data* add(glm::vec2 a, glm::vec2 b, glm::vec4 colour);
	void remove(Data* line);
}

//not done yet
namespace text {
	struct Data {
		Data* previous;
		Data* next;

		glm::vec2 pos;
		glm::vec4 colour;
		float size;
		const char* text;
		int length;
		int image;
	};

	/** adds a text job to the renderer, pos is the lower left corner, float is the size in display units,
	*	image is the texture that the text is found in, a 16 x 16 bitmap font with the data stored in the
	*	alpha stream.
	*/
	Data* add(glm::vec2 pos, glm::vec4 colour, float size, std::string text, int image);
	void remove(Data* text);
}

#endif
