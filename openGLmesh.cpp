#include "openGLmesh.h"

OpenGLMesh::OpenGLMesh() {

	/*
	setScale(1.0f, 1.0f, 1.0f);
	setRotation(0, 0, 1, 0);
	setTranslation(0, 0, 0);
	*/
	scale = glm::mat4(1.0f);
	rotation = glm::mat4(1.0f);
	translation = glm::mat4(1.0f);
	texture = "";
	floatsPerVertex = 3;
	floatsPerColor = 4;
	floatsPerUV = 0;
	floatsPerNormal = 0;
	shininess = 1.0f;
}

void OpenGLMesh::setScale(float x, float y, float z) {

	scale = glm::scale(glm::vec3(x, y, z));
}

void OpenGLMesh::setRotation(float rotation, float x, float y, float z) {

	this->rotation = glm::rotate(rotation, glm::vec3(x, y, z));
}

void OpenGLMesh::addRotation(float rotation, float x, float y, float z) {
	this->rotation *= glm::rotate(rotation, glm::vec3(x, y, z));
}

void OpenGLMesh::setTranslation(float x, float y, float z) {

	translation = glm::translate(glm::vec3(x, y, z));
}

void OpenGLMesh::setShininess(float factor) {
	if (factor > 1.0f) shininess = 1.0f;
	else if (factor < 0.0f) shininess = 0.0f;
	else shininess = factor;
}

float OpenGLMesh::getShininess() {
	return shininess;
}

void OpenGLMesh::printVertices() {

	int i = 1;
	int line = 0;
	std::cout << line << "  ";
	line++;
	for (auto point : vertices) {
		std::cout << point << " ";
		if (i % (floatsPerColor + floatsPerUV + floatsPerVertex) == 0) {
			std::cout << std::endl;
			std::cout << line << "  ";
			line++;
		}
		i++;
	}
}

void OpenGLMesh::printIndices() {

	int i = 1;
	for (auto point : indices) {
		std::cout << point << " ";
		if (i % 3 == 0) std::cout << std::endl;
		i++;
	}
}