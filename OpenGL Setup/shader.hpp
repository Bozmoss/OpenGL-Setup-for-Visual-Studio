#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Shader class
class Shader {
private:
	GLuint shaderProgram, vertexShader, fragmentShader;
	const char* vertexSource, * fragmentSource;

	std::string loadShaderSource(std::string path);
	bool shaderCompiled(GLuint shader);

public:
	bool setVertexShader(std::string vertexPath);
	bool setFragmentShader(std::string fragmentPath);
	GLint getUniformLocation(std::string name);
	GLint compile();
	void use();
};