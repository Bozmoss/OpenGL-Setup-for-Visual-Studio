#include "engine.hpp"

Engine::Engine(int width, int height, const char* windowName):
	width(width), height(height), windowName(windowName),
	cameraPos(0.0f, 0.0f, 3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f),
	lastX(width / 2.0f), lastY(height / 2.0f), yaw(-90.0f), pitch(0.0f), firstMouse(true) {}

int Engine::init() {
	// Initialise GLFW
	glfwInit();

	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Use the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For MacOS
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a window
	window = glfwCreateWindow(width, height, windowName, glfwGetPrimaryMonitor(), nullptr);

	// Check if window was created
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });
	setupOpenGLRendering();

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		float frameTime = (float)glfwGetTime();
		float deltaTime = frameTime - lastFrameTime;
		lastFrameTime = frameTime;

		processInput(window);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update(deltaTime);
		render(frameTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 1;
}

void Engine::processInput(GLFWwindow* window) {
	// Input processing logic

	float cameraSpeed = 2.5f * 0.0016f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}
}

void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (!engine) return;

	if (engine->firstMouse) {
		engine->lastX = xpos;
		engine->lastY = ypos;
		engine->firstMouse = false;
		return;
	}

	float xoffset = xpos - engine->lastX;
	float yoffset = engine->lastY - ypos;
	engine->lastX = xpos;
	engine->lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	engine->yaw += xoffset;
	engine->pitch += yoffset;

	engine->pitch = glm::clamp(engine->pitch, -89.0f, 89.0f);

	glm::vec3 front;
	front.x = cos(glm::radians(engine->yaw)) * cos(glm::radians(engine->pitch));
	front.y = sin(glm::radians(engine->pitch));
	front.z = sin(glm::radians(engine->yaw)) * cos(glm::radians(engine->pitch));
	engine->cameraFront = glm::normalize(front);
}

void Engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	static Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	float fov = glm::radians(45.0f);
	fov -= yoffset;
	if (fov < glm::radians(1.0f)) fov = glm::radians(1.0f);
	if (fov > glm::radians(45.0f)) fov = glm::radians(45.0f);
}

void Engine::setupOpenGLRendering() {
	// OpenGL setup logic

	shader.setVertexShader("vertex.glsl");
	shader.setFragmentShader("frag.glsl");
	shader.compile();

	sphere = new Sphere(shader, glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0, 0.0, 0.0), 1.0f, false);
	cube = new Cube(shader, glm::vec3(0.0, -2.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	light = new Sphere(shader, glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0), 0.25f, true);
}

void Engine::update(float dt) {
	// Update logic
}

void Engine::render(float frameTime) {
	// Render logic

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 lightColor(1.0f);

	shader.use();
	shader.setVec3("viewPos", cameraPos);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("lightPos", lightPos);
	shader.setVec3("lightColor", lightColor);

	light->render();
	sphere->render();
	cube->render();
}