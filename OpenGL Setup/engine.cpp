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
	window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);

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

	float cameraSpeed = 2.5f * 0.016f;

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
}

void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (!engine) return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
		engine->firstMouse = true;
		return;
	}

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

	float vertices[] = {
		// Positions
	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f,
	   -0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f
	};

	unsigned int indices[] = {
	0, 1, 2, 2, 3, 0, // Back face
	4, 5, 6, 6, 7, 4, // Front face
	0, 4, 7, 7, 3, 0, // Left face
	1, 5, 6, 6, 2, 1, // Right face
	3, 2, 6, 6, 7, 3, // Top face
	0, 1, 5, 5, 4, 0  // Bottom face
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Engine::update(float dt) {
	// Update logic
}

void Engine::render(float frameTime) {
	// Render logic

	shader.use();
	model = glm::rotate(glm::mat4(1.0f), frameTime, glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}