#ifndef OPENGLCONTROLLER_H
#define OPENGLCONTROLLER_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "openGLmesh.h"
#include "camera.h"
#include "light.h"

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

	// data about the registered mesh objects
	struct meshID {
		GLuint vao;
		GLuint vbos[2];
		GLuint nIndices;
		glm::mat4 model;
		glm::mat4 rotation;
		float shininess;
		GLuint texture;
	};

	struct FrameTime {
		float lastFrame;
		float _startTime; // "private"
		void tick() { _startTime = glfwGetTime(); }
		void tock() { lastFrame = glfwGetTime() - _startTime;}
	};

	public:

		Camera MainCamera;

		void Initialize(); //call in constructor?
		
		void AddShape(OpenGLMesh& mesh);
		void AddLight(Light light);
		bool RunScene();
		

	private:

		void SetVersionInfo();
		void CreateWindow();
		void AssignCallbackRoutines();

		GLuint BuildShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
		void checkShaderCompilation(GLuint& shaderID);
		void checkShaderLink(GLuint& shaderID);

		GLuint AddTexture(const char* fileName);
		void SetupLights();

		void Render();
		void ProcessKeyboardInput();
		void ProcessMouseInput();

#define ORTHO 1
#define	PERSPECTIVE 0
		void setProjection(bool orthogonal);
		
		// the active window used as a target reference for inputs and rendering
		GLFWwindow* window;

		// active linked shader program ID stored
		GLuint colorShader;
		GLuint textureShader;

		// holds mesh VAOs as created
		std::vector<meshID> meshIds;

		// hold lights attached on scene
		std::vector<Light> mLightingArray;
		
		// these are updated per scene
		glm::mat4 view;
		glm::mat4 projection;
		FrameTime frameTime;


		/* Callbacks for GLFW controls 
				glfw api needs pointer to a function, not an object, so these callbacks must be
				essentially outside of the object. Very anti-OOP
				I created its own static class that in turn updates a few private variables in
				OpenGL Control and flags for processing.
				This way at least access to them remains through controller only.
		*/

		class Callbacks {

			friend class OpenGL;

		private: 

			// mouse position
			// the static variables hold information from a single callback so that an object instance can reference them
			static bool firstMouseMovement;
			static bool mouseMovement;
			static float mouseLastX;
			static float mouseLastY;
			static float xMouseMovement;
			static float yMouseMovement;
			static void _mousePosition(GLFWwindow* window, double xpos, double ypos);

			// mouse scroll
			static bool mouseScrolling;
			static float scale;
			static void _mouseScroll(GLFWwindow* window, double xoffset, double yoffset);

			// window resize
			static void _resizeWindow(GLFWwindow* window, int width, int height);

			// key press
			static bool keyP_Toggle;
			static void _keyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
		};

};

#endif
