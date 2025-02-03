#include "engine.hpp"

Engine::Engine(int width, int height, const char* windowName):
	width(width), height(height), windowName(windowName) {
}

int Engine::init() {
	// Initialise GLFW
	glfwInit();

	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Use the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// For MacOS
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a window
	window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

	// Check if window was created
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}

	// Set the viewport
	glViewport(0, 0, width, height);

	// Setup callbacks
	glfwSetFramebufferSizeCallback(window, WindowResize);
	setupOpenGLRendering();

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		float frameTime = (float)glfwGetTime();
		float deltaTime = frameTime - lastFrameTime;
		lastFrameTime = frameTime;

		glfwPollEvents();
		processInput(window);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update(deltaTime);
		render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 1;
}

void WindowResize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	
	// Resize logic
}

void Engine::processInput(GLFWwindow* window) {
	// Input processing logic

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void Engine::setupOpenGLRendering() {
	// OpenGL setup logic
}

void Engine::update(float dt) {
	// Update logic
}

void Engine::render() {
	// Render logic
}