#include "shader.hpp"

#include <fstream>
#include <string>

bool Shader::setVertexShader(std::string vertexPath) {
	std::string source = loadShaderSource(vertexPath);
	vertexSource = source.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	if (!shaderCompiled(vertexShader)) return false;
	return true;
}

bool Shader::setFragmentShader(std::string fragmentPath) {
	std::string source = loadShaderSource(fragmentPath);
	fragmentSource = source.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	if (!shaderCompiled(fragmentShader)) return false;
	return true;
}

GLint Shader::compile() {
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLint Shader::getUniformLocation(std::string name) {
	return glGetUniformLocation(shaderProgram, name.c_str());
}

void Shader::use() {
	glUseProgram(shaderProgram);
}

std::string Shader::loadShaderSource(std::string path) {
	std::ifstream fileReader;
	fileReader.open(path, std::ios::binary);

	if (fileReader.is_open()) {
		fileReader.seekg(0, std::ios::beg);
		std::string line;
		std::string output;
		while (std::getline(fileReader, line)) {
			output += line + "\n";
		}
		output += '\0';

		return output;
	}
	else {
		std::cerr << "Failed to open file: " << path << std::endl;
	}

	return "";
}

bool Shader::shaderCompiled(GLuint shader) {
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}