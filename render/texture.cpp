#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../log.hpp"
#include "renderer.hpp"

using namespace std;

//taken from www.opengl-tutorial.org, I'll put in a more complete solution when I get internet back
namespace image {
	GLuint loadBMP(string path) {
		path = "textures/" + path + ".bmp";
		const char* imagepath = path.c_str();

		log(INFO, "Loading texture: " + string(imagepath));

		// Data read from the header of the BMP file
		unsigned char header[54];
		unsigned int dataPos;
		unsigned int imageSize;
		unsigned int width, height;
		// Actual RGB data
		unsigned char* data;

		// Open the file
		FILE* file = fopen(imagepath, "rb");

		if (!file) {
			log(ERROR, string(imagepath) + " could not be loaded.");
			return 0;
		}


		// Read the header, i.e. the 54 first bytes

		// If less than 54 bytes are read, problem
		if ( fread(header, 1, 54, file)!=54 ){
			log(ERROR, string(imagepath) + " is not a valid BMP file.");
			return 0;
		}

		// A BMP files always begins with "BM"
		if ( header[0]!='B' || header[1]!='M' ){
			log(ERROR, string(imagepath) + " is not a valid BMP file.");
			return 0;
		}

		// Make sure this is a 24bpp file
		if ( *(int*)&(header[0x1E])!=0  )         {log(ERROR, string(imagepath) + " is not a valid BMP file.");    return 0;}
		if ( *(int*)&(header[0x1C])!=24 )         {log(ERROR, string(imagepath) + " is not a valid BMP file.");    return 0;}

		// Read the information about the image
		dataPos    = *(int*)&(header[0x0A]);
		imageSize  = *(int*)&(header[0x22]);
		width      = *(int*)&(header[0x12]);
		height     = *(int*)&(header[0x16]);

		// Some BMP files are misformatted, guess missing information
		if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos==0)      dataPos=54; // The BMP header is done that way

		// Create a buffer
		data = new unsigned char [imageSize];

		// Read the actual data from the file into the buffer
		fread(data,1,imageSize,file);

		// Everything is in memory now, the file will be closed
		fclose (file);

		// Create one OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

		// OpenGL has now copied the data. Free our own version
		delete [] data;

		// Poor filtering, or ...
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// ... nice trilinear filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		CHECK_GL();

		// Return the ID of the texture we just created
		return textureID;
	}

	#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
	#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
	#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

	//DDS files are used more effectively by the GPU, use if possible to lower startup times
	GLuint loadDDS(string path){
		path = "textures/" + path + ".dds";
		const char* imagepath = path.c_str();

		unsigned char header[124];

		FILE *fp;

		/* try to open the file */
		fp = fopen(imagepath, "rb");
		if (fp == NULL){
			log(ERROR, string(imagepath) + " could not be opened");
			getchar();
			return 0;
		}

		/* verify the type of file */
		char filecode[4];
		fread(filecode, 1, 4, fp);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			fclose(fp);
			return 0;
		}

		/* get the surface desc */
		fread(&header, 124, 1, fp);
	
		unsigned int height      = *(unsigned int*)&(header[8 ]);
		unsigned int width	     = *(unsigned int*)&(header[12]);
		unsigned int linearSize	 = *(unsigned int*)&(header[16]);
		unsigned int mipMapCount = *(unsigned int*)&(header[24]);
		unsigned int fourCC      = *(unsigned int*)&(header[80]);
	
		unsigned int bufsize;
		/* how big is it going to be including all mipmaps? */
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		unsigned char* buffer = new unsigned char[bufsize];
		fread(buffer, 1, bufsize, fp);
		/* close the file pointer */
		fclose(fp);
 
		unsigned int format;
		switch(fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			delete [] buffer;
			return 0;
		}

		// Create one OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	
		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for(unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
	 
			offset += size;
			width  /= 2;
			height /= 2;

			// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
			if(width < 1)
				width = 1;

			if(height < 1)
				height = 1;

		}

		delete [] buffer;

		CHECK_GL();

		return textureID;
	}
}
