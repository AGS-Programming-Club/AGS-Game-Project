#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

//taken from www.opengl-tutorial.org
namespace image {
	// Load a .BMP file using our custom loader
	GLuint loadBMP(std::string name);

	// Load a .DDS file using GLFW's own loader
	GLuint loadDDS(std::string name);
}

#endif
