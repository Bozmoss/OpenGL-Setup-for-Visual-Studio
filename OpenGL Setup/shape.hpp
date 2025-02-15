#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

class Shape {
protected:
	GLuint VAO, VBO, EBO;
	Shader& shader;
	glm::vec3 color, pos;

public:
	Shape(Shader& shader, glm::vec3 pos, glm::vec3 color) : shader(shader), pos(pos), color(color) {}
	virtual ~Shape() {}

	virtual void setupMesh() = 0;
	virtual void render() = 0;
};

class Cube : public Shape {
private:
    float vertices[8 * 6 * 6] = {
        // Positions                // Normals
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // Back face
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // Front face
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,

        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // Left face
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,

         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // Right face
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // Top face
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // Bottom face
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f
    };

    float verticesInverted[8 * 6 * 6] = {
        // Positions                // Normals
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  1.0f, // Back face
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f,  1.0f,

        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, -1.0f, // Front face
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f, -1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // Left face
        -0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,

         0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // Right face
         0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // Top face
         0.5f,  0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, -1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // Bottom face
         0.5f, -0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  1.0f,  0.0f
    };

    unsigned int indices[36] = {
        0, 1, 2, 2, 3, 0, // Back face
        4, 5, 6, 6, 7, 4, // Front face
        8, 9, 10, 10, 11, 8, // Left face
        12, 13, 14, 14, 15, 12, // Right face
        16, 17, 18, 18, 19, 16, // Top face
        20, 21, 22, 22, 23, 20  // Bottom face
    };

    bool invert;
    float scale;

public:
	Cube(Shader& shader, glm::vec3 pos, glm::vec3 color, float scale, bool invert) : Shape(shader, pos, color), scale(scale), invert(invert) {
		setupMesh();
	}

	void setupMesh() override;

	void render() override;
};

class Sphere : public Shape {
private:
    static const unsigned int latitudeSegments = 40, longitudeSegments = 40;

    float vertices[(latitudeSegments + 1) * (longitudeSegments + 1) * 6], radius;
    unsigned int indices[(latitudeSegments + 1) * (longitudeSegments + 1) * 6];

    bool isLight;

public:
    Sphere(Shader& shader, glm::vec3 pos, glm::vec3 color, float radius, bool isLight) : Shape(shader, pos, color), radius(radius), isLight(isLight) {
        setupMesh();
    }

    void setupMesh() override;

    void render() override;
};