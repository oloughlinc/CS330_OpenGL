#include "openGLcontroller.h"
#include "shaders.h"

#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define WINDOW_TITLE "CS330 Final Project - CO"

// BG COLOR
const struct {
	// this is grey 95%
	float R = 0.05f;
	float G = 0.05f;
	float B = 0.05f;
	float A = 1.0f;
} DEFAULT_COLOR;

void OpenGL::Initialize() {

	// Initialize GLFW library, for drawing windows and context
	if (!glfwInit()) throw std::runtime_error("GLFW did not initialize properly");
	SetVersionInfo();
	CreateWindow();

	// Initialize GLEW library, for drawing shapes
	if (glewInit() != GLEW_OK) throw std::runtime_error("GLEW did not initialize properly");
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	AssignCallbackRoutines();

	// Build shaders
	//this->colorShader = BuildShaderProgram(vertexShaderSource, colorShaderSource);
	this->textureShader = BuildShaderProgram(vertexShaderSource, textureShaderSource);
}

void OpenGL::SetVersionInfo() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGL::CreateWindow() {

	// glfw create window, set as the current context, and assign a callback operation
	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == nullptr) throw std::runtime_error("Failed to create GLFW window");
	glfwMakeContextCurrent(window);

	// Set default (background) color
	glClearColor(DEFAULT_COLOR.R, DEFAULT_COLOR.G, DEFAULT_COLOR.B, DEFAULT_COLOR.A);

	// Initialize mouse control
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void OpenGL::AssignCallbackRoutines() {
	glfwSetFramebufferSizeCallback(window, Callbacks::_resizeWindow);
	glfwSetCursorPosCallback(window, Callbacks::_mousePosition);
	glfwSetScrollCallback(window, Callbacks::_mouseScroll);
	glfwSetKeyCallback(window, Callbacks::_keyPress);
}

GLuint OpenGL::BuildShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {

	// Create a shader program object, we store the ref id in a GL_uint
	GLuint shaderProgramID = glCreateProgram();

	// create vertex and shader objects, and store the respective ids
	auto vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	auto fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// pull shader sources from shader.h
	glShaderSource(vertShaderId, 1, &vertexShaderSource, NULL);
	glShaderSource(fragShaderId, 1, &fragmentShaderSource, NULL);

	// compile shaders
	glCompileShader(vertShaderId);
	glCompileShader(fragShaderId);

	// check that shaders compiled OK
	checkShaderCompilation(vertShaderId);
	checkShaderCompilation(fragShaderId);
	
	// attach shaders
	glAttachShader(shaderProgramID, vertShaderId);
	glAttachShader(shaderProgramID, fragShaderId);

	// link shader program and check for issues
	glLinkProgram(shaderProgramID);
	checkShaderLink(shaderProgramID);

	return shaderProgramID;
}

void OpenGL::checkShaderCompilation(GLuint& shaderID) {

	int success;
	char infoLog[512];
	char errMessage[1024] = "SHADER COMPILATION FAILED\n";

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		strcat_s(errMessage, infoLog);
		throw std::runtime_error(errMessage);
	}

}

void OpenGL::checkShaderLink(GLuint& shaderID) {

	int success;
	char infoLog[512];
	char errMessage[1024] = "SHADER PROGRAM LINKING FAILED\n";

	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		strcat_s(errMessage, infoLog);
		throw std::runtime_error(errMessage);
	}

}

void OpenGL::AddLight(Light light) {
	mLightingArray.push_back(light);
}

void OpenGL::AddShape(OpenGLMesh& meshInfo) {

	// create a new vertex array
	meshID newMesh;
	glGenVertexArrays(1, &newMesh.vao);
	glBindVertexArray(newMesh.vao);

	//determine the byte size of the data in meshInfo vectors (for GPU buffer)
	auto vertByteSize = sizeof(GLfloat) * meshInfo.vertices.size();
	auto indexByteSize = sizeof(GLuint) * meshInfo.indices.size();

	// create two vbos for array
	glGenBuffers(2, newMesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.vbos[0]);     
	glBufferData(GL_ARRAY_BUFFER, vertByteSize, &(meshInfo.vertices)[0], GL_STATIC_DRAW); // send to GPU memory
	// &(meshInfo.vertices)[0] = pointer to vectors backing array https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array/2923295#2923295

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteSize, &(meshInfo.indices)[0], GL_STATIC_DRAW); // send to GPU memory

	// ---- Create the Vertex Attribute Pointers ----
	// (these are referenced by the shaders) 

	//size in memory between each point/color combo defines stride
	GLint stride = sizeof(float) * (meshInfo.floatsPerVertex + meshInfo.floatsPerColor + meshInfo.floatsPerUV + meshInfo.floatsPerNormal);
 
	// location 0 starts at 0, moves length of stride, and picks floatspervertex = 3 values each
	glVertexAttribPointer(0, meshInfo.floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// location 1 starts after floatsperindex=3 (times memory size), moves length of stride, and picks floatspercolor=4 values each
	glVertexAttribPointer(1, meshInfo.floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * meshInfo.floatsPerVertex));
	glEnableVertexAttribArray(1);

	// generate texture if there is one
	newMesh.texture = 0;
	if (meshInfo.texture != "") {

		newMesh.texture = AddTexture(meshInfo.texture.c_str());

		// make texture info from this mesh available to shader
		glVertexAttribPointer(2, meshInfo.floatsPerUV, GL_FLOAT, GL_FALSE, stride,
			(void*)(sizeof(float) * (meshInfo.floatsPerVertex + meshInfo.floatsPerColor)));
		glEnableVertexAttribArray(2);
	}

	// location 3 will hold model normals
	glVertexAttribPointer(3, meshInfo.floatsPerNormal, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(float) * (meshInfo.floatsPerVertex + meshInfo.floatsPerColor + meshInfo.floatsPerUV)));
	glEnableVertexAttribArray(3);

	// link some data from the meshInfo that will be needed at render
	newMesh.model = meshInfo.model();
	newMesh.nIndices = meshInfo.nIndices();
	newMesh.rotation = meshInfo.getRotation();
	newMesh.shininess = meshInfo.getShininess();
	
	this->meshIds.push_back(newMesh); //add object to render queue
}

GLuint OpenGL::AddTexture(const char* fileName) {

	GLuint textureId;
	int width, height, channels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(fileName, &width, &height, &channels, 0);
	if (!image) throw std::runtime_error("Could not load image");

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set the texture wrapping parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	else
		throw std::runtime_error("Cannot handle channels other than 3 or 4");

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture.

	return textureId;
}

void OpenGL::setProjection(bool orthogonal) {

	if (orthogonal) {
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	}
	else {
		projection = glm::perspective(MainCamera.getFieldOfView(), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	}
}

bool OpenGL::RunScene() {

	SetupLights();

	while (!glfwWindowShouldClose(window)) {

		frameTime.tick();
		   Render();
		frameTime.tock();

		glfwPollEvents(); // poll input and other
		ProcessMouseInput();
		ProcessKeyboardInput();
	}
	return true;
}

void OpenGL::SetupLights() {

	GLuint shaderProgramID = textureShader; //color shader has no lighting or just ambient lights
	glUseProgram(shaderProgramID); // set openGL to use our linked shader program

	// get locations
	GLint lightColorLoc1 = glGetUniformLocation(shaderProgramID, "lightColor1");
	GLint lightPositionLoc1 = glGetUniformLocation(shaderProgramID, "lightPos1");
	GLint lightStrengthLoc1 = glGetUniformLocation(shaderProgramID, "lightStrength1");
	GLint lightColorLoc2 = glGetUniformLocation(shaderProgramID, "lightColor2");
	GLint lightPositionLoc2 = glGetUniformLocation(shaderProgramID, "lightPos2");
	GLint lightStrengthLoc2 = glGetUniformLocation(shaderProgramID, "lightStrength2");

	// handle lighting information
	if (mLightingArray.size() < 1) {
		// let pass for now. Could throw an exception too "no lights no lights".
	}
	if (mLightingArray.size() >= 1) { // add actual data
		// attach
		glUniform3f(lightColorLoc1, mLightingArray.at(0).R, mLightingArray.at(0).G, mLightingArray.at(0).B);
		glUniform3f(lightPositionLoc1, mLightingArray.at(0).X, mLightingArray.at(0).Y, mLightingArray.at(0).Z);
		glUniform1f(lightStrengthLoc1, mLightingArray.at(0).getIntensity());
	}
	else { // add a light that is "off"
			// more efficient to create multiple shaders but this is fine for small scenes
		glUniform3f(lightColorLoc1, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPositionLoc1, 0.0f, 0.0f, 0.0f);
		glUniform1f(lightStrengthLoc1, 0.0f);
	}
	if (mLightingArray.size() >= 2) {
		glUniform3f(lightColorLoc2, mLightingArray.at(1).R, mLightingArray.at(1).G, mLightingArray.at(1).B);
		glUniform3f(lightPositionLoc2, mLightingArray.at(1).X, mLightingArray.at(1).Y, mLightingArray.at(1).Z);
		glUniform1f(lightStrengthLoc2, mLightingArray.at(1).getIntensity());
	}
	else {
		glUniform3f(lightColorLoc2, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPositionLoc2, 0.0f, 0.0f, 0.0f);
		glUniform1f(lightStrengthLoc2, 0.0f);
	}
	if (mLightingArray.size() >= 3) {
		// not supported actually
	}

}

void OpenGL::Render() {

	glEnable(GL_DEPTH_TEST); // automatically resolve pixel color based on depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear screen

	// get the camera view for this frame
	view = MainCamera.getView();

	for (auto meshID : meshIds) { //for each created and registered mesh, which has its own VAO

		bool isTextured = (meshID.texture != 0) ? true : false;
		GLuint shaderProgramID = isTextured ? textureShader : colorShader;

		
		glUseProgram(shaderProgramID); // set openGL to use our linked shader program
		glUniform1i(glGetUniformLocation(shaderProgramID, "uTexture"), 0);

		// Retrieves and passes transform matrices to the shader program
		GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
		GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
		GLint rotationLoc = glGetUniformLocation(shaderProgramID, "rotation");
		GLint shinyLoc = glGetUniformLocation(shaderProgramID, "shininess");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(meshID.model)); //each mesh has its own model, and uses the general view and P
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(meshID.rotation)); // we will rotate the normals in the shader
		glUniform1f(shinyLoc, meshID.shininess);

		// attach extra information for speculars
		GLint objectColorLoc = glGetUniformLocation(shaderProgramID, "objectColor");
		GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");
		glUniform3f(viewPositionLoc, MainCamera.mCameraPosition.x, MainCamera.mCameraPosition.y, MainCamera.mCameraPosition.z);

		if (isTextured) {
			glUniform1i(glGetUniformLocation(shaderProgramID, "uTexture"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, meshID.texture);
		}

		glBindVertexArray(meshID.vao); // set openGL to use our mesh array (it was registered in the library at creation)
		glDrawElements(GL_TRIANGLES, meshID.nIndices, GL_UNSIGNED_SHORT, NULL); // DRAW (as triangles)
		glBindVertexArray(0); // unset this VAO

		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
	glfwSwapBuffers(window); // send this frame to window and move active window information back
}


void OpenGL::ProcessMouseInput() {
	// the static variables for movement are updated in the callback routine for mouse movement
	// so now we have to make sure that we process the new movement ourselves
	if (Callbacks::mouseMovement) {
		MainCamera.rotate(Callbacks::xMouseMovement, Callbacks::yMouseMovement);
		Callbacks::mouseMovement = false; 
	}
	if (Callbacks::mouseScrolling) {
		MainCamera.changeMovementSpeed(Callbacks::scale);
		Callbacks::mouseScrolling = false;
	}
}

void OpenGL::ProcessKeyboardInput() {

	// ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	// CAMERA DIRECTIONALS
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		MainCamera.move(CameraDirection::FORWARD, frameTime.lastFrame);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		MainCamera.move(CameraDirection::BACK, frameTime.lastFrame);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		MainCamera.move(CameraDirection::LEFT, frameTime.lastFrame);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		MainCamera.move(CameraDirection::RIGHT, frameTime.lastFrame);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		MainCamera.move(CameraDirection::UP, frameTime.lastFrame);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		MainCamera.move(CameraDirection::DOWN, frameTime.lastFrame);

	// P
	if (Callbacks::keyP_Toggle) setProjection(ORTHO);
	else setProjection(PERSPECTIVE);
}


/* GLFW :: Callbacks -----------------------------------------------------------------------------------*/

// GLFW: whenever the window size changed (by OS or user resize) this callback function executes
void OpenGL::Callbacks::_resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// mouse tracking
bool OpenGL::Callbacks::firstMouseMovement = true;
bool OpenGL::Callbacks::mouseMovement = false;
float OpenGL::Callbacks::mouseLastX;
float OpenGL::Callbacks::mouseLastY;
float OpenGL::Callbacks::xMouseMovement;
float OpenGL::Callbacks::yMouseMovement;
void OpenGL::Callbacks::_mousePosition(GLFWwindow* window, double xpos, double ypos) {

	// set the initial position the first time the mouse is moved
	if (firstMouseMovement) {
		mouseLastX = xpos;
		mouseLastY = ypos;
		firstMouseMovement = false;
	}

	// get the delta mouse movement from last call in each dimension and update in controller
	xMouseMovement = xpos - mouseLastX;
	yMouseMovement = mouseLastY - ypos;
	mouseLastX = xpos;
	mouseLastY = ypos;

	//flag movement for the controller
	mouseMovement = true;
}

// mouse scrolling
bool OpenGL::Callbacks::mouseScrolling = false;
float OpenGL::Callbacks::scale;
void OpenGL::Callbacks::_mouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	scale = yoffset;
	mouseScrolling = true;
}

// key callback looks for key P downpress
bool OpenGL::Callbacks::keyP_Toggle = false;
void OpenGL::Callbacks::_keyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		keyP_Toggle = !keyP_Toggle;
	}
}


