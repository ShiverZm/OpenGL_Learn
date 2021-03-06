#include <iostream>

#include <stdlib.h>
#include <string.h>



#include <glad/glad.h>
#include <GLFW/glfw3.h>




#pragma comment(lib,"glfw3dll.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew.lib")



static char* shaderLoadSource(const char* filePath)
{
	const size_t blockSize = 512;
	FILE* fp;
	char buf[blockSize];
	char* source = NULL;
	size_t tmp, sourceLength = 0;

	fp = fopen(filePath, "r");
	if (!fp) {
		fprintf(stderr, "shaderLoadSource():Unable to open %s for reading\n", filePath);
		return NULL;
	}

	while ((tmp = fread(buf, 1, blockSize, fp)) > 0)
	{
		char* newSource = (char*)malloc(sourceLength + tmp + 1);
		if (!newSource) {
			fprintf(stderr, "shaderLoadSource():malloc failed\n");
			if (source) {
				free(source);
			}
			return NULL;
		}
		if (source) {
			memcpy(newSource, source, sourceLength);
			free(source);
		}
		memcpy(newSource + sourceLength, buf, tmp);
		source = newSource;
		sourceLength += tmp;
	}
	fclose(fp);
	if (source) {
		source[sourceLength] = '\0';
	}

	return source;
}

static GLuint shaderCompileFromFile(GLenum type, const char* filePath)
{
	char* source;
	GLuint shader;
	GLint length, result;

	source = shaderLoadSource(filePath);
	if (!source) {
		return 0;
	}
	shader = glCreateShader(type);
	length = strlen(source);
	glShaderSource(shader, 1, (const char**)&source, &length);
	glCompileShader(shader);
	free(source);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char* log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = (char*)malloc(length);
		glGetShaderInfoLog(shader, length, &result, log);
		fprintf(stderr, "shaderCompileFromFile(): Unable to compile %s: %s\n", filePath, log);
		free(log);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void shaderAttachFromFile(GLuint program, GLenum type, const char* filePath)
{
	GLuint shader = shaderCompileFromFile(type, filePath);
	if (shader != 0) {
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}
}


void main() {
	shaderLoadSource("F:\\workspace\\project\\github\\cpp\\obs-studio\\plugins\\obs-filters\\data\\chroma_key_filter.effect");
}