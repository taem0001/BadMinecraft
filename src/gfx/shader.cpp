#include "../../include/gfx/shader.h"
#include <iostream>

using namespace std;

namespace Minecraft {
	namespace GFX {
		GLuint _compile(const char *path, GLenum type) {
			// Open shader file and save its contents in a string
			FILE *fp = fopen(path, "r");
			if (!fp) {
				fprintf(stderr, "[ERROR] Failed to open file.\n");
				exit(-1);
			}

			char *buf;

			fseek(fp, 0, SEEK_END);
			int size = ftell(fp);
			int n = size / sizeof(char);

			buf = (char *)malloc(sizeof(char) * n);
			if (!buf) {
				fprintf(stderr, "[ERROR] Failed to allocate memory.\n");
				exit(-1);
			}

			fseek(fp, 0, 0);
			int c, i = 0;
			while (EOF != (c = getc(fp))) {
				buf[i] = c;
				i++;
			}
			fclose(fp);

			// Compile the shader in OpenGL
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &buf, NULL);
			glCompileShader(shader);

			// Check if shader compiled successfully
			int success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				fprintf(stderr, "[ERROR] Failed to compile shader.\n");
			}

			free(buf);
			return shader;
		}

		void Shader::init(const char *vspath, const char *fspath) {
			GLuint vertex = _compile(vspath, GL_VERTEX_SHADER);
			GLuint fragment = _compile(fspath, GL_FRAGMENT_SHADER);

			// Create shader program
			handle = glCreateProgram();
			glAttachShader(handle, vertex);
			glAttachShader(handle, fragment);
			glLinkProgram(handle);

			// Check shader linking success
			int success;
			glGetProgramiv(handle, GL_LINK_STATUS, &success);
			if (!success) {
				fprintf(stderr, "[ERROR] Failed to link shader.\n");
			}

			// Delete the fragment and vertex shaders as they are no longer
			// needed
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		Shader::~Shader() { glDeleteProgram(handle); }

		void Shader::use() { glUseProgram(handle); }

		void Shader::setBool(const char *name, bool value) const {
			glUniform1i(glGetUniformLocation(handle, name), (int)value);
		}

		void Shader::setInt(const char *name, int value) const {
			glUniform1i(glGetUniformLocation(handle, name), value);
		}

		void Shader::setFloat(const char *name, float value) const {
			glUniform1f(glGetUniformLocation(handle, name), value);
		}
	} // namespace GFX
} // namespace Minecraft
