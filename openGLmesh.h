#ifndef OPENGLMESH_H
#define OPENGLMESH_H

#include <GL\glew.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

class OpenGLMesh {

	public:


		std::vector<GLfloat> vertices;
		std::vector<GLushort> indices;

		GLuint floatsPerVertex;
		GLuint floatsPerColor;
		GLuint floatsPerUV;
		GLuint floatsPerNormal;
		std::string texture;
		

		GLuint nIndices() { return indices.size(); }

		glm::mat4 model() { return translation * rotation * scale; }

		void setScale(float x, float y, float z);
		void setRotation(float rotation, float x, float y, float z);
		void addRotation(float rotation, float x, float y, float z);
		glm::mat4 getRotation() { return rotation; }
		void setTranslation(float x, float y, float z);

		void setShininess(float factor);
		float getShininess();

		// mostly for debugging
		void printVertices();
		void printIndices();

		OpenGLMesh();

	private:

		glm::mat4 scale;
		glm::mat4 rotation;
		glm::mat4 translation;

		float shininess;

		
};


#endif
