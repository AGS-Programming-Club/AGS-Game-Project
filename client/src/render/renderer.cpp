//GLEW openGL Extension Wrangler Library
#include <GL/glew.h>
//GLFW used for keyboard IO and window creation
#include <GLFW/glfw3.h>

#include <string.h>
#include <glm/glm.hpp>
#include <list>
#include <map>

#include "log.hpp"
#include "render/renderer.hpp"
#include "render/programLoader.hpp"

using namespace std;
using namespace glm;

namespace render {
	GLuint colour_mat;
	GLuint text_mat;
	GLuint text_sampler;
	GLuint texture_mat;
	GLuint texture_sampler;
}

//All data is considered persistant and is kept in a single buffer per render type, dynamic data will be added latter

/** Holds methods used to render solid triangles */
namespace solidTriangle {
	int DATA_SIZE_FLOAT = 18;

	GLuint program;

	void init(GLuint vbo, GLuint vao) {
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position
		glEnableVertexAttribArray(1); //used for color, texturing will be added later if required

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (2 * sizeof(float)));
	}

	void clean() {
		//glDeleteProgram(program); removed so the the solid colour program is not deleted twice
	}
}

/** Holds methods used to render solid triangles */
namespace texturedTriangle {
	int DATA_SIZE_FLOAT = 12;

	GLuint program = 0;

	void init(GLuint vbo, GLuint vao) {
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position
		glEnableVertexAttribArray(1); //used for color, texturing will be added later if required

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
	}

	void clean() {
		glDeleteProgram(program);
	}
}

/** Holds methods used to render text */
namespace text {
	//for the technically inclined text is rendererd using instancing

	/** whether the GPU buffer needs to be updated next frame */
	bool update = false;

	int DATA_SIZE_FLOAT = 8;

	GLuint staticVbo;
	GLuint program = 0;


	const float vertexs[] = {
			0, 0,
			1, 0,
			1, 1,

			1, 1,
			0, 1,
			0, 0,
	};

	void initStaticBuffer() {
		glGenBuffers(1, &staticVbo);
		glBindBuffer(GL_ARRAY_BUFFER, staticVbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexs, GL_STATIC_DRAW);
	}

	void init(GLuint vao, GLuint vbo) {
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position of vertexs

		glEnableVertexAttribArray(1); //used for letter code
		glEnableVertexAttribArray(2); //used for the position of the letter
		glEnableVertexAttribArray(3); //used for colour
		glEnableVertexAttribArray(4); //used for the size of the font

		glBindBuffer(GL_ARRAY_BUFFER, staticVbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 32, (void*) 0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, (void*) 4);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 32, (void*) (4 + 8));
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 32, (void*) (4 + 8 + 16));

		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
	}

	void clean() {
		glDeleteBuffers(1, &staticVbo);
		glDeleteProgram(program);
	}

	int getCharCode(char c) {
		return (int) c - 32;
	}

	int getLetterLength(list<RenderJob::TextData*> texture) {
		int acc = 0;
		for(auto line : texture) {
			acc += line->length;
		}

		return acc;
	}
}

/** Holds methods used to render coloured lines */
namespace line {
	const int DATA_SIZE_FLOAT = 12;

	GLuint program = 0;

	void init(GLuint vao, GLuint vbo) {
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position
		glEnableVertexAttribArray(1); //used for color, texturing will be added later if required

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (2 * sizeof(float)));
	}

	void clean() {
		glDeleteProgram(program);
	}
}

//holds rendering methods
namespace render {
	list<RenderJob*> jobs;
	RenderJob* ui;
	RenderJob* world;

	RenderJob* getWorldJob() {
		return world;
	}

	RenderJob* getUIJob() {
		return ui;
	}

	RenderJob* createRenderJob() {
		RenderJob* job = new RenderJob();
		jobs.push_back(job);
		return job;
	}

	void deleteRenderJob(RenderJob* job) {
		jobs.remove(job);
		delete job;
	}

	bool checkGL(string message) {
		GLenum err = glGetError();
		if(err != GL_NO_ERROR) {
			log(ERROR, message + string((const char*) (gluErrorString(err))));
			return true;
		}

		return false;
	}

	/** creates and initializes all of the opengl programs needed */
	void createPrograms() {
		GLuint colourVertex = shader::create(GL_VERTEX_SHADER, "colourvertex");
		GLuint colourFragment = shader::create(GL_FRAGMENT_SHADER, "colourfragment");

		GLuint textureFragment = shader::create(GL_FRAGMENT_SHADER, "texturefragment");
		GLuint textureVertex = shader::create(GL_VERTEX_SHADER, "texturevertex");

		GLuint textVertex = shader::create(GL_VERTEX_SHADER, "textvertex");
		GLuint textFragment = shader::create(GL_FRAGMENT_SHADER, "textfragment");

		CHECK_GL();

		GLuint colourArray [] = {colourVertex, colourFragment};
		GLuint textureArray [] = {textureVertex, textureFragment};
		GLuint textArray [] = {textVertex, textFragment};

		line::program = program::create(colourArray);
		solidTriangle::program = line::program;
		text::program = program::create(textArray);
		texturedTriangle::program = program::create(textureArray);

		render::colour_mat = glGetUniformLocation(line::program, "matrix");

		render::text_mat = glGetUniformLocation(text::program, "matrix");
		render::text_sampler = glGetUniformLocation(text::program, "image");

		render::texture_mat = glGetUniformLocation(texturedTriangle::program, "matrix");
		render::texture_sampler = glGetUniformLocation(texturedTriangle::program, "image");

		CHECK_GL();
	}

	GLFWwindow* window;

	GLFWwindow* getWindow() {
		return window;
	}

	/* Creates a window with Opengl versions major.minor
 	 * if context creation fails or you need more features submit a pull
 	 * request for a different version number ect...
 	 */
	void init(int major, int minor, int samples, string title, bool fullscreen) {
		if(!glfwInit()) {
			log(ERROR, "Failed to initialize GLFW");
			return;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_SAMPLES, samples); //multisampling

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //setting Opengl version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //needed for MAC users

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits); //attempting to avoid driver switches
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		window = glfwCreateWindow(mode->width / (fullscreen ? 1 : 2), mode->height / (fullscreen ? 1 : 2), title.c_str(), fullscreen ? monitor : NULL, NULL);
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE; //needed to make opengl work, weird

		if(glewInit() != GLEW_OK) {
			log(ERROR, "Error in glew initializtion");
			return;
		}

		glGetError(); //this line prevents it by flushing the error buffers.
		CHECK_GL(); //this line always returns an error, not sure why

		createPrograms();

		CHECK_GL();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		text::initStaticBuffer();

		CHECK_GL();

		ui = createRenderJob();
		world = createRenderJob();
	}

	void draw() {
		for(auto job : jobs) {
			job->draw();
		}
	}

	/* Swaps buffers and renders the queued shapes */
	void tick() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* Returns true if the the window has been closed */
	bool shouldClose() {
		return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
	}

	void clean() {
		line::clean();
		solidTriangle::clean();
		texturedTriangle::clean();

		if(!CHECK_GL())
			log(INFO, "OpenGL has cleaned up sucessfully.");

		glfwTerminate();
	}
}

namespace texture {
	void bind(GLuint image, int unit) {
		//unit < 80 or programs may crash on low spec cards

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, image);
	}
}

//class implementaions, first time with c++ classes please let me know if I am doing it wrong
RenderJob::SolidTriangleData* RenderJob::addSolidTriangle(vec2 a, vec2 b, vec2 c, vec4 colour) {
	SolidTriangleData* d = new RenderJob::SolidTriangleData;
	d->a = a;
	d->b = b;
	d->c = c;
	d->colour = colour;

	solidTriangles.push_back(d);
	updateSolidTriangle = true;

	return d;
}

void RenderJob::removeSolidTriangle(SolidTriangleData* data) {
	solidTriangles.remove(data);

	delete data;
	updateSolidTriangle = true;
}

mat3& RenderJob::getCameraMatrix() {
	return matrix;
}

void RenderJob::remakeSolidTriangleBuffer() {
	//TODO use mapped buffers
	GLfloat data[solidTriangles.size() * solidTriangle::DATA_SIZE_FLOAT];

	std::list<SolidTriangleData*>::const_iterator iterator;
	int i = 0;
	for(iterator = solidTriangles.begin(); iterator != solidTriangles.end(); ++iterator) {
		//vert 1
		data[i++] = (*iterator)->a.x;
		data[i++] = (*iterator)->a.y;

		data[i++] = (*iterator)->colour.r;
		data[i++] = (*iterator)->colour.g;
		data[i++] = (*iterator)->colour.b;
		data[i++] = (*iterator)->colour.a;

		//vert 2
		data[i++] = (*iterator)->b.x;
		data[i++] = (*iterator)->b.y;

		data[i++] = (*iterator)->colour.r;
		data[i++] = (*iterator)->colour.g;
		data[i++] = (*iterator)->colour.b;
		data[i++] = (*iterator)->colour.a;

		//vert 3
		data[i++] = (*iterator)->c.x;
		data[i++] = (*iterator)->c.y;

		data[i++] = (*iterator)->colour.r;
		data[i++] = (*iterator)->colour.g;
		data[i++] = (*iterator)->colour.b;
		data[i++] = (*iterator)->colour.a;
	}

	glBindBuffer(GL_ARRAY_BUFFER, solidTriangleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	updateSolidTriangle = false;
}

/** Draws the current queue onto the currently bound framebuffer */
void RenderJob::draw() {
	updateCamera();

	if(solidTriangles.size() != 0)
		drawSolidTriangles();

	if(lines.size() != 0)
		drawLines();

	if(totalTexturedSize() != 0)
		drawTexturedTriangles();

	if(letters != 0)
		drawText();
}

void RenderJob::drawSolidTriangles() {
	if(updateSolidTriangle)
		remakeSolidTriangleBuffer();

	glBindVertexArray(solidTriangleVao);
	glUseProgram(solidTriangle::program);
	glDrawArrays(GL_TRIANGLES, 0, solidTriangles.size() * 3);
}

int RenderJob::totalTexturedSize() {
	int acc = 0;
	for(auto l : texturedTriangles) {
		acc += l.second.size();
	}

	return acc;
}

RenderJob::TexturedTriangleData* RenderJob::addTexturedTriangle(vec2 a, vec2 b, vec2 c, vec2 p, vec2 q, vec2 r, int texture) {
	if(texturedTriangles.find(texture) == texturedTriangles.end()) {
		texturedTriangles[texture] = list<TexturedTriangleData*>();
	}

	TexturedTriangleData* d = new TexturedTriangleData;
	d->a = a;
	d->b = b;
	d->c = c;
	d->p = p;
	d->q = q;
	d->r = r;
	d->texture = texture;

	texturedTriangles[texture].push_back(d);

	updateTexturedTriangle = true;

	return d;
}

void RenderJob::removeTexturedTriangle(TexturedTriangleData* data) {
	texturedTriangles[data->texture].remove(data);

	delete data;

	updateTexturedTriangle = true;
}

void RenderJob::remakeTexturedTriangleBuffer() {
	int size = totalTexturedSize();

	//TODO use mapped buffers
	GLfloat data[size * texturedTriangle::DATA_SIZE_FLOAT];

	for(auto l1 : texturedTriangles) {
		int i = 0;
		for(auto l2 : l1.second) {
			//vert 1
			data[i++] = l2->a.x;
			data[i++] = l2->a.y;

			data[i++] = l2->p.s;
			data[i++] = l2->p.t;

			//vert 2
			data[i++] = l2->b.x;
			data[i++] = l2->b.y;

			data[i++] = l2->q.s;
			data[i++] = l2->q.t;

			//vert 2
			data[i++] = l2->c.x;
			data[i++] = l2->c.y;

			data[i++] = l2->r.s;
			data[i++] = l2->r.t;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, texturedTriangleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	updateTexturedTriangle = false;
}

void RenderJob::drawTexturedTriangles() {
	if(updateTexturedTriangle)
		remakeTexturedTriangleBuffer();

	glBindVertexArray(texturedTriangleVao);
	glUseProgram(texturedTriangle::program);

	int acc = 0;
	for(auto i : texturedTriangles) {
		if(i.second.size() == 0)
			continue;

		glUniform1i(render::texture_sampler, i.first);
		glDrawArrays(GL_TRIANGLES, acc, i.second.size() * 3);
		acc += i.second.size();
	}
}

RenderJob::TextData* RenderJob::addText(vec2 pos, vec4 colour, float size, string text, int image) {
	if(textJobs.find(image) == textJobs.end()) {
		textJobs[image] = list<TextData*>();
	}

	TextData* d = new TextData;
	d->supply = new StringLetterSupplier{size, size * 0.375f, size, colour, pos, text};
	d->length = text.length();
	d->image = image;

	letters += d->length;

	textJobs[image].push_back(d);

	updateText = true;

	return d;
}

RenderJob::TextData* RenderJob::addText(LetterSupplier* supply, int image, int length) {
	if(textJobs.find(image) == textJobs.end()) {
		textJobs[image] = list<TextData*>();
	}

	TextData* d = new TextData;
	d->supply = supply;
	d->length = length;
	d->image = image;

	letters += d->length;

	textJobs[image].push_back(d);

	updateText = true;

	return d;
}

void RenderJob::removeText(TextData* data) {
	letters -= data->length;

	textJobs[data->image].remove(data);

	delete data->supply;
	delete data;

	updateText = true;
}

void RenderJob::remakeTextBuffer() {
	//TODO use mapped buffers
	GLfloat data[letters * text::DATA_SIZE_FLOAT];

	int i = 0;

	for(auto texture : textJobs) {
		for(auto line : texture.second) {
			int q = 0;
			LetterSupplier::Letter letter;
			for(int q = 0; q < line->length; q++) {
				letter = line->supply->next(q == 0 ? NULL : &letter, q);
				((int*) data)[i++] = text::getCharCode(letter.charcode);

				data[i++] = letter.pos.x;
				data[i++] = letter.pos.y;

				data[i++] = letter.colour.r;
				data[i++] = letter.colour.g;
				data[i++] = letter.colour.b;
				data[i++] = letter.colour.a;

				data[i++] = letter.scale;
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, textVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	updateText = false;
}

void RenderJob::drawText() {
	if(updateText)
		remakeTextBuffer();

	glBindVertexArray(textVao);
	glUseProgram(text::program);

	int acc = 0;
	for(auto texture : textJobs) {
		if(texture.second.size() == 0)
			continue;

		glUniform1i(render::text_sampler, texture.first);
		int f = text::getLetterLength(texture.second);
		glDrawArraysInstanced(GL_TRIANGLES, acc, 6, f);
		acc += f;
	}
}

RenderJob::LineData* RenderJob::addLine(vec2 a, vec2 b, vec4 colour) {
	LineData* d = new LineData;
	d->a = a;
	d->b = b;
	d->colour = colour;

	lines.push_front(d);

	updateLine = true;
	return d;
}

void RenderJob::removeLine(LineData* data) {
	lines.remove(data);

	delete data;

	updateLine = true;
}

void RenderJob::remakeLineBuffer() {
	//TODO use mapped buffers
	GLfloat data[lines.size() * line::DATA_SIZE_FLOAT];

	int i = 0;
	list<LineData*>::const_iterator iterator;
	for(iterator = lines.begin(); iterator != lines.end(); ++iterator) {
		//vert 1
		data[i++] = (*iterator)->a.x;
		data[i++] = (*iterator)->a.y;

		data[i++] = (*iterator)->colour.r;
		data[i++] = (*iterator)->colour.g;
		data[i++] = (*iterator)->colour.b;
		data[i++] = (*iterator)->colour.a;

		//vert 2
		data[i++] = (*iterator)->b.x;
		data[i++] = (*iterator)->b.y;

		data[i++] = (*iterator)->colour.r;
		data[i++] = (*iterator)->colour.g;
		data[i++] = (*iterator)->colour.b;
		data[i++] = (*iterator)->colour.a;
	}

	glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	updateLine = false;
}

void RenderJob::drawLines() {
	if(updateLine)
		remakeLineBuffer();

	glBindVertexArray(lineVao);
	glUseProgram(line::program);
	glDrawArrays(GL_LINES, 0, lines.size() * 2);
}

void RenderJob::updateCamera() {
	glUseProgram(texturedTriangle::program);
	glUniformMatrix3fv(render::texture_mat, 1, GL_FALSE, &matrix[0][0]);

	glUseProgram(text::program);
	glUniformMatrix3fv(render::text_mat, 1, GL_FALSE, &matrix[0][0]);

	glUseProgram(line::program);
	glUniformMatrix3fv(render::colour_mat, 1, GL_FALSE, &matrix[0][0]);
}

LetterSupplier::Letter StringLetterSupplier::next(Letter* previous, int index) {
	Letter letter;

	if(index >= text.length()) {
		letter.charcode = ' ';
	} else {
		letter.charcode = text.at(index);
	}

	letter.colour = colour;

	if(previous != NULL) {
		vec2 oldPos = previous->pos;
		switch(letter.charcode) {
			case '\t':
				letter.pos = vec2(oldPos.x + 3 * width, oldPos.y);
				break;
			case '\n':
				letter.pos = vec2(startPos.x - width, oldPos.y - height);
				break;
			default:
				letter.pos = vec2(oldPos.x + width, oldPos.y);
		}
	} else {
		letter.pos = startPos;
	}

	letter.scale = scale;

	return letter;
}

StringLetterSupplier::StringLetterSupplier(float height, float width, float scale, glm::vec4 colour, glm::vec2 startPos, std::string text) {
	this->height = height;
	this->width = width;
	this->scale = scale;
	this->colour = colour;
	this->startPos = startPos;
	this->text = text;
}

RenderJob::RenderJob() {
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

	glGenBuffers(1, &lineVbo);
	glGenVertexArrays(1, &lineVao);
	glGenBuffers(1, &textVbo);
	glGenVertexArrays(1, &textVao);
	glGenBuffers(1, &texturedTriangleVbo);
	glGenVertexArrays(1, &texturedTriangleVao);
	glGenBuffers(1, &solidTriangleVbo);
	glGenVertexArrays(1, &solidTriangleVao);

	line::init(lineVao, lineVbo);
	texturedTriangle::init(texturedTriangleVbo, texturedTriangleVao);
	solidTriangle::init(solidTriangleVbo, solidTriangleVao);
	text::init(textVao, textVbo);
}
