#pragma once

// System libraries
#include <iostream>

// Include libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Header includes
// TODO: Add headers here

// Function prototypes
void WindowResize(GLFWwindow* window, int width, int height);

// Class definition
class Engine {
private:
	int width = 800, height = 600;
	const char* windowName = "Name";
	GLFWwindow* window;

	// Game loop
	float lastFrameTime = 0.0f;
	glm::vec3 clearColor = glm::vec3(0.0f);

	// OpenGL
	void setupOpenGLRendering();

public:
	float gameSpeed = 0.1f;

	Engine(int width, int height, const char* windowName);

	int init();
	void update(float dt);
	void render();
	void processInput(GLFWwindow* window);
};