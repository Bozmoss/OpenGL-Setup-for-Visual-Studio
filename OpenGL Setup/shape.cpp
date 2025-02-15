#include "shape.hpp"

void Cube::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::render() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);

    shader.setMat4("model", model);

    shader.setVec3("objectColor", color);

    glUniform1i(shader.getUniformLocation("isLight"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::setupMesh() {
    float pi = 3.14159265359f;
    float twoPi = 6.28318530718f;

    unsigned int count = 0;
    for (unsigned int lat = 0; lat <= latitudeSegments; lat++) {
        float theta = lat * pi / latitudeSegments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (unsigned int lon = 0; lon <= longitudeSegments; lon++) {
            float phi = lon * twoPi / longitudeSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = radius * sinTheta * cosPhi;
            float y = radius * cosTheta;
            float z = radius * sinTheta * sinPhi;

            vertices[count++] = x;
            vertices[count++] = y;
            vertices[count++] = z;
            vertices[count++] = x; // Normals
            vertices[count++] = y;
            vertices[count++] = z;
        }
    }

    count = 0;
    for (unsigned int lat = 0; lat <= latitudeSegments; lat++) {
        for (unsigned int lon = 0; lon <= longitudeSegments; lon++) {
            unsigned int first = (lat * (longitudeSegments + 1)) + lon, second = first + longitudeSegments + 1;

            indices[count++] = first;
            indices[count++] = second;
            indices[count++] = first + 1;
            indices[count++] = second;
            indices[count++] = second + 1;
            indices[count++] = first + 1;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::render() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);

    shader.setMat4("model", model);

    shader.setVec3("objectColor", color);

    if (isLight) {
        glUniform1i(shader.getUniformLocation("isLight"), 1);
    }
    else {
        glUniform1i(shader.getUniformLocation("isLight"), 0);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}