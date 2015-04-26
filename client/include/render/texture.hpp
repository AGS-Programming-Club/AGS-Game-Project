#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

//taken from www.opengl-tutorial.org
namespace image {
	/** Load a .BMP file using the loader from www.opengl-tutorial.org. The extension and /textures can be ommited. */
	GLuint loadBMP(std::string name);

	/** Load a .DDS file using GLFW's own loader. The extension and /textures can be ommited */
	GLuint loadDDS(std::string name);
}

#endif
