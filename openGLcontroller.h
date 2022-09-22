#ifndef OPENGLCONTROLLER_H
#define OPENGLCONTROLLER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "openGLmesh.h"

#include <vector>

/*
* Wrapper class to control the GLEW and GLFW libraries. Some of its own state information is saved in its own fields for reference.
* The basic idea is to initialize OpenGL in a set way, and easily add objects to render from a seperate function. 
* GLEW = model, this = controller
* 
* The control will create each mesh VBO and buffer on GPU whenever AddShape is called. It will further add the created mesh VAO ID to
* the list. The list is iterated each frame at render time, drawing all objects in order. Object position and rotation is set outside the controller,
* and this -model- is combined with view and projection in the vertex shader at render.
* 
* Therefore AddShape can only accept OpenGLMesh objects as they contain model information also (uses glm).
* 
*/

class OpenGL {

	struct meshID {
		GLuint vao;
		GLuint vbos[2];
		GLuint nIndices;
		glm::mat4 model;
	};

	public:

		void Initialize(); //call in constructor?
		void BuildShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
		void AddShape(OpenGLMesh& mesh);
		void setCameraPos(float x, float y, float z);
		void setFieldOfView(float angle);
		bool RunScene();
		

	private:

		void checkShaderCompilation(GLuint& shaderID);
		void checkShaderLink();
		void Render();
		//void resizeWindowCallback(GLFWwindow* window, int width, int height); not part of class but in the file. Is this bad?

		GLFWwindow* window;

		GLuint shaderProgramID;
		std::vector<meshID> meshIds;

		glm::mat4 view;
		glm::mat4 projection;

};

#endif
