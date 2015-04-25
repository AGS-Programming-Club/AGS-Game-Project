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
	vec2 position = vec2(0, 0);
	vec2 scale = vec2(1, 1);

	GLuint colour_c_pos;
	GLuint colour_c_scale;
	GLuint text_c_pos;
	GLuint text_c_scale;
	GLuint text_sampler;
	GLuint texture_c_pos;
	GLuint texture_c_scale;
	GLuint texture_sampler;

	vec2* getCameraPos() {
		return &position;
	}

	vec2* getCameraScale() {
		return &scale;
	}
}

//All data is considered persistant and is kept in a single buffer per render type, dynamic data will be added latter

/** Holds methods used to render solid triangles */
namespace solidTriangle {
	/** whether the GPU buffer needs to be updated next frame */
	bool update = false;

	int DATA_SIZE_FLOAT = 18;

	GLuint vao;
	GLuint vbo;
	GLuint program;

	list<Data*> triangles;

	Data* add(vec2 a, vec2 b, vec2 c, vec4 colour) {
		Data* d = new Data;
		d->a = a;
		d->b = b;
		d->c = c;
		d->colour = colour;

		triangles.push_back(d);

		update = true;

		return d;
	}

	void remove(Data* data) {
		triangles.remove(data);

		delete data;
		update = true;
	}

	void init() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position
		glEnableVertexAttribArray(1); //used for color, texturing will be added later if required

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (2 * sizeof(float)));
	}

	void clean() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		//glDeleteProgram(program); removed so the the solid colour program is not deleted twice
	}

	void remakeBuffer() {
		//TODO use mapped buffers
		GLfloat data[triangles.size() * DATA_SIZE_FLOAT];

		list<Data*>::const_iterator iterator;
		int i = 0;
		for(iterator = triangles.begin(); iterator != triangles.end(); ++iterator) {
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

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		update = false;
	}

	void draw() {
		if(update)
			remakeBuffer();

		glBindVertexArray(vao);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
	}
}

/** Holds methods used to render solid triangles */
namespace texturedTriangle {
	/** whether the GPU buffer needs to be updated next frame */
	bool update = false;

	int DATA_SIZE_FLOAT = 12;

	GLuint vao;
	GLuint vbo;
	GLuint program = 0;

	map<int, list<Data*>> textures;

	int totalSize() {
		int acc = 0;
		for(auto l : textures) {
			acc += l.second.size();
		}

		return acc;
	}

	Data* add(vec2 a, vec2 b, vec2 c, vec2 p, vec2 q, vec2 r, int texture) {
		if(textures.find(texture) == textures.end()) {
			textures[texture] = list<Data*>();
		}

		Data* d = new Data;
		d->a = a;
		d->b = b;
		d->c = c;
		d->p = p;
		d->q = q;
		d->r = r;
		d->texture = texture;

		textures[texture].push_back(d);

		update = true;

		return d;
	}

	void remove(Data* data) {
		textures[data->texture].remove(data);

		delete data;

		update = true;
	}

	void init() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position
		glEnableVertexAttribArray(1); //used for color, texturing will be added later if required

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
	}

	void clean() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

	void remakeBuffer() {
		int size = totalSize();

		//TODO use mapped buffers
		GLfloat data[size * DATA_SIZE_FLOAT];

		for(auto l1 : textures) {
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

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		update = false;
	}

	void draw() {
		if(update)
			remakeBuffer();

		glBindVertexArray(vao);
		glUseProgram(program);

		int acc = 0;
		for(auto i : textures) {
			if(i.second.size() == 0)
				continue;

			glUniform1i(render::texture_sampler, i.first);
			glDrawArrays(GL_TRIANGLES, acc, i.second.size() * 3);
			acc += i.second.size();
		}
	}
}

/** Holds methods used to render text */
namespace text {
	//for the technically inclined text is rendererd using instancing

	/** whether the GPU buffer needs to be updated next frame */
	bool update = false;

	int DATA_SIZE_FLOAT = 8;

	GLuint vao;
	GLuint vbo;
	GLuint staticVbo;
	GLuint program = 0;

	map<int, list<Data*>> textures;
	int letters = 0;

	//NB arrays need to be updated whenever the number of textures change
	int numberOfTextures = 0; //the number of textures

	Data* add(vec2 pos, vec4 colour, float size, string text, int image) {
		if(textures.find(image) == textures.end()) {
			textures[image] = list<Data*>();
		}

		Data* d = new Data;
		d->pos = pos;
		d->colour = colour;
		d->size = size;
		d->text = text.c_str();
		d->image = image;
		d->length = text.length();

		letters += d->length;

		textures[image].push_back(d);

		update = true;

		return d;
	}

	void remove(Data* data) {
		letters -= data->length;

		textures[data->image].remove(data);

		delete data;

		update = true;
	}

	const float vertexs[] = {
			0, 0,
			1, 0,
			1, 1,

			1, 1,
			0, 1,
			0, 0,
	};

	void init() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &staticVbo);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position of vertexs

		glEnableVertexAttribArray(1); //used for letter code
		glEnableVertexAttribArray(2); //used for the position of the letter
		glEnableVertexAttribArray(3); //used for colour
		glEnableVertexAttribArray(4); //used for the size of the font

		glBindBuffer(GL_ARRAY_BUFFER, staticVbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertexs, GL_STATIC_DRAW);
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
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &staticVbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

	int getCharCode(char c) {
		return (int) c - 32;
	}

	const float WIDTH_SCALE = 0.5;

	void remakeBuffer() {
		//TODO use mapped buffers
		GLfloat data[letters * DATA_SIZE_FLOAT];

		int i = 0;

		for(auto texture : textures) {
			for(auto line : texture.second) {
				int q = 0;
				for(char& c : line->text) {
					((int*) data)[i++] = getCharCode(c);

					data[i++] = line->pos.x + line->size * q++ * WIDTH_SCALE; //this assumes unispace font
					data[i++] = line->pos.y;

					data[i++] = line->colour.r;
					data[i++] = line->colour.g;
					data[i++] = line->colour.b;
					data[i++] = line->colour.a;

					data[i++] = line->size;
				}
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		update = false;
	}

	int getLetterLength(list<Data*> texture) {
		int acc = 0;
		for(auto line : texture) {
			acc += line->text.length();
		}

		return acc;
	}

	void draw() {
		if(update)
			remakeBuffer();

		glBindVertexArray(vao);
		glUseProgram(program);

		int acc = 0;
		for(auto texture : textures) {
			if(texture.second.size() == 0)
				continue;

			glUniform1i(render::text_sampler, texture.first);
			int f = getLetterLength(texture.second);
			glDrawArraysInstanced(GL_TRIANGLES, acc, 6, f);
			acc += f;
		}
	}
}

/** Holds methods used to render coloured lines */
namespace line {
	/** whether the GPU buffer needs to be updated next frame */
	bool update = false;
	const int DATA_SIZE_FLOAT = 16;

	GLuint vao;
	GLuint vbo;
	GLuint program = 0;

	/** The top of the linked list implementation */
	list<Data*> lines;

	Data* add(vec2 a, vec2 b, vec4 colour) {
		Data* d = new Data;
		d->a = a;
		d->b = b;
		d->colour = colour;

		lines.push_front(d);

		update = true;
		return d;
	}

	void remove(Data* data) {
		lines.remove(data);

		delete data;

		update = true;
	}

	void init() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0); //used for position
		glEnableVertexAttribArray(1); //used for color, texturing will be added later if required

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (2 * sizeof(float)));
	}

	void clean() {
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(program);
	}

	void remakeBuffer() {
		//TODO use mapped buffers
		GLfloat data[lines.size() * DATA_SIZE_FLOAT];

		int i = 0;
		list<Data*>::const_iterator iterator;
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

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		update = false;
	}

	void draw() {
		if(update)
			remakeBuffer();

		glBindVertexArray(vao);
		glUseProgram(program);
		glDrawArrays(GL_LINES, 0, lines.size() * 2);
	}
}

//holds rendering methods
namespace render {
	bool cameraChange = true;

	void updateCamera() {
		cameraChange = true;
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

		render::colour_c_pos = glGetUniformLocation(line::program, "c_pos");
		render::colour_c_scale = glGetUniformLocation(line::program, "c_scale");

		render::text_c_pos = glGetUniformLocation(text::program, "c_pos");
		render::text_c_scale = glGetUniformLocation(text::program, "c_scale");
		render::text_sampler = glGetUniformLocation(text::program, "image");

		render::texture_c_pos = glGetUniformLocation(texturedTriangle::program, "c_pos");
		render::texture_c_scale = glGetUniformLocation(texturedTriangle::program, "c_scale");
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

		line::init();
		solidTriangle::init();
		texturedTriangle::init();
		text::init();
	}

	void draw();

	void updateCamera_impl() {
		glUseProgram(texturedTriangle::program);
		glUniform2f(render::texture_c_pos, position.x, position.y);
		glUniform2f(render::texture_c_scale, scale.x, scale.y);

		glUseProgram(text::program);
		glUniform2f(render::text_c_pos, position.x, position.y);
		glUniform2f(render::text_c_scale, scale.x, scale.y);

		glUseProgram(line::program);
		glUniform2f(render::colour_c_pos, position.x, position.y);
		glUniform2f(render::colour_c_scale, scale.x, scale.y);

		cameraChange = false;
	}

	/* Swaps buffers and renders the queued shapes */
	void tick() {
		if(cameraChange) {
			updateCamera_impl();
		}

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

	/** Draws the current queue onto the currently bound framebuffer */
	void draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(solidTriangle::triangles.size() != 0)
			solidTriangle::draw();

		if(line::lines.size() != 0)
			line::draw();

		if(texturedTriangle::totalSize() != 0)
			texturedTriangle::draw();

		if(text::letters != 0)
			text::draw();
	}
}

namespace texture {
	void bind(GLuint image, int unit) {
		//unit < 80 or programs may crash on low spec cards

		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, image);
	}
}
