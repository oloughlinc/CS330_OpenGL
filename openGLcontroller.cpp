#include "openGLcontroller.h"
#include "configurables.h"

#include <stdexcept>
#include <iostream>


// GLFW: whenever the window size changed (by OS or user resize) this callback function executes
void _resizeWindowCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OpenGL::Initialize() {

	// Initialize GLFW library, for drawing windows and context
	if (!glfwInit()) throw std::runtime_error("GLFW did not initialize properly");

	// set version information
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw create window, set as the current context, and assign a callback operation
	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == nullptr) throw std::runtime_error("Failed to create GLFW window");
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, _resizeWindowCallback);

	// Initialize GLEW library, for drawing shapes
	if (glewInit() != GLEW_OK) throw std::runtime_error("GLEW did not initialize properly");

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Set default (background) color
	glClearColor(DEFAULT_COLOR.R, DEFAULT_COLOR.G, DEFAULT_COLOR.B, DEFAULT_COLOR.A);

	// Set default view
	setCameraPos(0.0f, 0.0f, -5.0f);
	setFieldOfView(45.0f);
}

void OpenGL::BuildShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {

	// Create a shader program object, we store the ref id in a GL_uint
	this->shaderProgramID = glCreateProgram();

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
	checkShaderLink();

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

void OpenGL::checkShaderLink() {

	int success;
	char infoLog[512];
	char errMessage[1024] = "SHADER PROGRAM LINKING FAILED\n";

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(infoLog), NULL, infoLog);
		strcat_s(errMessage, infoLog);
		throw std::runtime_error(errMessage);
	}

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
	GLint stride = sizeof(float) * (meshInfo.floatsPerVertex + meshInfo.floatsPerColor);
 
	// location 0 starts at 0, moves length of stride, and picks floatspervertex = 3 values each
	glVertexAttribPointer(0, meshInfo.floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	// location 1 starts after floatsperindex=3 (times memory size), moves length of stride, and picks floatspercolor=4 values each
	glVertexAttribPointer(1, meshInfo.floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * meshInfo.floatsPerVertex));
	glEnableVertexAttribArray(1);

	// pull some more data from the meshInfo that will be needed at render
	newMesh.model = meshInfo.model();
	newMesh.nIndices = meshInfo.nIndices();

	this->meshIds.push_back(newMesh); //add object to render queue
}

void OpenGL::setCameraPos(float x, float y, float z) {
	view = glm::translate(glm::vec3(x, y, z));
}

void OpenGL::setFieldOfView(float angle) {
	projection = glm::perspective(angle, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
}

bool OpenGL::RunScene() {
	while (!glfwWindowShouldClose(window)) {
		Render();
		glfwPollEvents();
	}
	return true;
}

void OpenGL::Render() {

	glEnable(GL_DEPTH_TEST); // automatically resolve pixel color based on depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear screen

	for (auto meshID : meshIds) { //for each created and registered mesh, which has its own VAO

		glUseProgram(shaderProgramID); // set openGL to use our linked shader program

		// Retrieves and passes transform matrices to the shader program
		GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
		GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(meshID.model)); //each mesh has its own model, and uses the general view and P
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(meshID.vao); // set openGL to use our mesh array (it was registered in the library at creation)
		glDrawElements(GL_TRIANGLES, meshID.nIndices, GL_UNSIGNED_SHORT, NULL); // DRAW (as triangles)

		glBindVertexArray(0); // unset this VAO
		
	}

	glfwSwapBuffers(window); // send this frame to window and move active window information back
	

}


