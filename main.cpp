#include <iostream>
#include <stdexcept>

#include "openGLcontroller.h"
#include "shaders.h"
#include "shapes.h"

int main() {

	// Init OpenGL Controller
	OpenGL GLControl;
	GLControl.Initialize(); 
	GLControl.BuildShaderProgram(vertexShaderSource, fragmentShaderSource);

	/*
	Add two cubes, scale to rectangles, rotate them slightly differently, and stack them
	on top of each other. This will be a representation of the stack of cards in
	the selected 2D scene.

	The vertices and colors are defined in the shapes namespace as functions that return OpenGLMesh objects.
	*/

	// define shape to send to scene
	OpenGLMesh bottomCards = shapes::Cube();
	bottomCards.setScale(1.6f, 0.5f, 1.4f);
	bottomCards.setRotation(45.0f, 0.0f, 1.0f, 0.0f);
	bottomCards.setTranslation(-2.0f, 0.0f, 0.0f);
	// add to scene
	GLControl.AddShape(bottomCards); 
	
	OpenGLMesh topCards = shapes::Cube();
	topCards.setScale(1.6f, 0.5f, 1.4f);
	topCards.setRotation(44.9f, 0.0f, 1.0f, 0.0f);
	topCards.setTranslation(-2.0f, 0.5f, 0.0f);
	GLControl.AddShape(topCards);

	/*
	* Create one plane that will serve to ground the scene
	*/

	OpenGLMesh basePlane = shapes::Plane();
	basePlane.setScale(8.0f, 8.0f, 8.0f);
	basePlane.setRotation(0.0f, 1.0f, 0.0f, 0.0f);
	basePlane.setTranslation(-4.0f, 0.0f, 2.0f);
	GLControl.AddShape(basePlane);
	
	// set view and projection parameters
	GLControl.setCameraPos(0.0f, -1.5f, -5.0f);
	GLControl.setFieldOfView(45.0f); 

	// render loop
	GLControl.RunScene();
	return 0;
}
