#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "../log.hpp"
#include "renderer.hpp"

using namespace std;

//taken from http://www.opengl-tutorial.org/ with some modifications

namespace shader {
	GLuint create(GLuint type, string path) {
		GLuint ID = glCreateShader(type);

		// Read the Vertex Shader code from the file
		string VertexShaderCode;
		ifstream VertexShaderStream(("./shaders/" + path + ".glsl").c_str(), ios::in);

		if(VertexShaderStream.is_open()){
			string Line = "";

			while(getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;

			VertexShaderStream.close();
		} else {
			log(ERROR, "Cannot read shader file: ./shaders/" + path + ".glsl");
			return 0;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		log(INFO, "Compiling shader: " + string(path));
		const char* VertexSourcePointer = VertexShaderCode.c_str();

		glShaderSource(ID, 1, &VertexSourcePointer , NULL); //null indicates that the string it NULL terminated
		glCompileShader(ID);

		// Check the shader
		glGetShaderiv(ID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if(InfoLogLength > 0) {
			char message[InfoLogLength + 1];
			glGetShaderInfoLog(ID, InfoLogLength, NULL, message);
			log(INFO, "Shader log: " + string(message));
		}

		//CHECK_GL();

		return ID;
	}
}

namespace program {
	GLuint create(GLuint* shaders, int length = 2) {
		CHECK_GL();

		GLuint ID = glCreateProgram();

		for(int i = 0; i < length; i++) {
			glAttachShader(ID, shaders[i]);
		}

		CHECK_GL();

		glLinkProgram(ID);

		GLint result = GL_FALSE;
		int infoLogLength;

		// Check the program

		glGetProgramiv(ID, GL_LINK_STATUS, &result);
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0){
			char infoLog[infoLogLength + 1];
			glGetProgramInfoLog(ID, infoLogLength, NULL, infoLog);
			log(INFO, "Program log: " + string(infoLog));
		}

		CHECK_GL();

		for(int i = 0; i < length; i++)
			glDeleteShader(shaders[i]);

		CHECK_GL();

		return ID;
	}
}
