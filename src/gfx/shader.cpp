#include "../../include/gfx/shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

namespace Minecraft {
	namespace GFX {
		long filelength(FILE *file) {
			long numbytes;
			long savedpos = ftell(file);
			fseek(file, 0, SEEK_END);
			numbytes = ftell(file);
			fseek(file, savedpos, SEEK_SET);
			return numbytes;
		}

		GLuint _compile(const char *path, GLenum type) {
			// Open shader file and save to string
			FILE *file = fopen(path, "r");
			if (!file) {
				std::cerr << "[ERROR] Failed to open shader file." << std::endl;
				exit(EXIT_FAILURE);
			}
			int bytesinfile = filelength(file);
			unsigned char *buf = (unsigned char *)malloc(bytesinfile + 1);
			if (!buf) {
				std::cerr << "[ERROR] Failed to allocate memory." << std::endl;
			}

			int bytesread = fread(buf, 1, bytesinfile, file);
			buf[bytesread] = '\0';
			fclose(file);

			// Compile the shader
			GLuint shader = glCreateShader(type);
			const char *shaderstrings[1];
			shaderstrings[0] = (char *)buf;
			glShaderSource(shader, 1, shaderstrings, NULL);
			glCompileShader(shader);
			free((void *)buf);

			// Check if compilation succeeded
			int success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				std::cerr << "[ERROR] Failed to compile shader." << std::endl;
			}

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
				std::cerr << "[ERROR] Failed to link shader." << std::endl;
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

		void Shader::setMat4(const char *name, glm::mat4 matrix) const {
			glUniformMatrix4fv(glGetUniformLocation(handle, name), 1, GL_FALSE,
							   glm::value_ptr(matrix));
		}
	} // namespace GFX
} // namespace Minecraft
