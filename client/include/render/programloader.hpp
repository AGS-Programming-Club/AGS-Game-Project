#ifndef SHADERLOADER_HPP
#define SHADERLOADER_HPP

#include <string>
#include <GL/glew.h>

namespace shader {
	/** Loads a shader from the name given and compiles it.
	 * @param type The opengl enum specifying the type of shader
	 * @param path The name of the shader, leave out the extension and /shaders/
	 * @returns an opengl ID for the object created*/
	GLuint create(GLuint type, std::string path);
}

namespace program {
	/** Links a render program object given an array of shaders
	 * @param shaders, an array of shaders
	 * @param (optional) length the length of the array, 2 if the default value
	 *
	 * @returns an opengl ID for the object created */
	GLuint create(GLuint* shaders, int length = 2);
}

#endif
