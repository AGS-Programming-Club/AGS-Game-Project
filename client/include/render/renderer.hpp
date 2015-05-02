#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cstring>
#include <list>
#include <map>
#include <glm/glm.hpp>

#define SFILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define CHECK_GL() render::checkGL("GL Error " + std::string(SFILE) + "." + LINE_STRING + ": ")

class RenderJob {
	public:
		struct SolidTriangleData {
			glm::vec2 a;
			glm::vec2 b;
			glm::vec2 c;
			glm::vec4 colour;
		};

		struct TexturedTriangleData {
			glm::vec2 a;
			glm::vec2 b;
			glm::vec2 c;

			glm::vec2 p;
			glm::vec2 q;
			glm::vec2 r;

			int texture; //this may be removed later
		};

		struct LineData {
			glm::vec2 a;
			glm::vec2 b;
			glm::vec4 colour;
		};

		struct TextData {
			glm::vec2 pos;
			glm::vec4 colour;
			float size;
			std::string text;
			int length;
			int image;
		};

	private:
		glm::mat3 matrix;

		bool updateSolidTriangle;
		bool updateTexturedTriangle;
		bool updateText;
		bool updateLine;

		std::list<SolidTriangleData*> solidTriangles;
		std::map<int, std::list<TexturedTriangleData*>> texturedTriangles;
		std::map<int, std::list<TextData*>> textJobs;
		std::list<LineData*> lines;

		void remakeSolidTriangleBuffer();
		void remakeTexturedTriangleBuffer();
		void remakeLineBuffer();
		void remakeTextBuffer();

		void drawSolidTriangles();
		void drawTexturedTriangles();
		void drawLines();
		void drawText();

		void updateCamera();

		int totalTexturedSize();
		int letters;

	public:
		RenderJob() {
			letters = 0;
			solidTriangles = std::list<SolidTriangleData*>();
			texturedTriangles = std::map<int, std::list<TexturedTriangleData*>>();
			textJobs = std::map<int, std::list<TextData*>>();
			lines = std::list<LineData*>();

			updateSolidTriangle = false;
			updateTexturedTriangle = false;
			updateText = false;
			updateLine = false;

			matrix = glm::mat3(1.0);
		}

		/** For internal use only, do not use.
		 **/
		void draw();

		/** @returns the matrix used for camera transformations, if you change the value it will update automatically
		 * on the GPU.
		 **/
		glm::mat3* getCameraMatrix();

		/** Adds a static solid flat shaded triangle to the renderer queue
		 *
		 * @param a a point
		 * @param b a point
		 * @param c yet another point, these must be in counterclockwise order
		 * @param colour the colour of the triangle */
		SolidTriangleData* addSolidTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec4 colour);

		/** removes a triangle using the pointer returned in @see solidTriangle::add */
		void removeSolidTriangle(SolidTriangleData* triangle);

		/** Adds a static textured triangle to the renderer queue
		 *
		 * @param a a point
		 * @param b a point
		 * @param c yet another point, these must be in counterclockwise order
		 *
		 * @param p Texture coordinates for a
		 * @param q b
		 * @param r and c
		 *
		 * @param texture the texture index to draw from */
		TexturedTriangleData* addTexturedTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 p, glm::vec2 q, glm::vec2 r, int texture);

		void removeTexturedTriangle(TexturedTriangleData* triangle);

		/** Adds a static line to the renderer queue
		 *
		 * @param a a point
		 * @param b a point
		 * @param colour the colour of the line */
		LineData* addLine(glm::vec2 a, glm::vec2 b, glm::vec4 colour);

		void removeLine(LineData* line);

		/** Adds a text job to the renderer, this is not to be used for bulk objects that move, that will cause performance issues.
		 * @param pos the lower left corner of the text in world coordinates
		 * @param colour the colour of the text in rgba.
		 * @param text the string to be rendererd, NB: carrabge returns and tabbs are not yet supported
		 * @param image The image index that the text bitmap was loaded into, @see texture::bind()*/
		TextData* addText(glm::vec2 pos, glm::vec4 colour, float size, std::string text, int image);

		/** Removes a text job via the handle returned in @see text::add
		 * @param the data returned from @see text::add */
		void removeText(TextData* text);
};

namespace render {
	/** @returns the RenderJob used for displaying non-moving world elements
	 **/
	RenderJob* getWorldJob();

	/** @returns the RenderJob used for non-moving geometry like GUIs
	 **/
	RenderJob* getUIJob();

	/** Creates a renderJob with a specific camera transform
	 * @returns created RenderJob object
	 **/
	RenderJob* createRenderJob();

	//TODO possible overloading of delete
	/** Deletes a RenderJob from the queue, all data will be lost
	 **/
	void deleteRenderJob(RenderJob* job);

	/** @returns the window object */
	GLFWwindow* getWindow();

	/** Creates the opengl context and displays the window.
	 * @param major The major version of opengl used.
	 * @param minor The minor version of opengl used.
	 * @param title The title to display on the window.
	 * @param fullscreen whether to make the window fullscreen, if it is not the size is
	 *  set to half the size of the screen. */
	void init(int major, int minor, int samples, std::string title, bool fullscreen);

	/** Renders all items and updates buffers if neccessary.
	 **/
	void tick();

	/** @returns true if the OS has signaled the window to close or escape has been pressed
	 **/
	bool shouldClose();

	/** Cleans the memory on the graphics card, forgetting to do this is very very bad
	 */
	void clean();

	/** Checks the opengl driver for errors, this also clears the error buffer
	 *
	 * @returns true is there was an error, the error is then logged
	 */
	bool checkGL(std::string message);
}

namespace texture {
	/** binds an image to a specified texture unit that can be used in @see texturedTriangle::add and @see text::add.
	 * 	This overwrites any old bind to this texture unit
	 *
	 * @param image The opengl id of the image.
	 * @param the id to bind the image to, this must be below 32. */
	void bind(GLuint image, int unit);
}
#endif
