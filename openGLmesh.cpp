#include "openGLmesh.h"

OpenGLMesh::OpenGLMesh() {
	setScale(1.0f, 1.0f, 1.0f);
	setRotation(0, 0, 0, 0);
	setTranslation(0, 0, 0);
}

void OpenGLMesh::setScale(float x, float y, float z) {

	scale = glm::scale(glm::vec3(x, y, z));
}

void OpenGLMesh::setRotation(float rotation, float x, float y, float z) {

	this->rotation = glm::rotate(rotation, glm::vec3(x, y, z));
}

void OpenGLMesh::setTranslation(float x, float y, float z) {

	translation = glm::translate(glm::vec3(x, y, z));
}
