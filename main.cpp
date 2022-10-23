#include <iostream>
#include <stdexcept>

#include "openGLcontroller.h"
#include "shapes.h"

int main() {

	// Init OpenGL Controller
	OpenGL GLControl;
	try {
		GLControl.Initialize();
	}
	catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	/*
	Add two cubes, scale to rectangles, rotate them slightly differently, and stack them
	on top of each other. This will be a representation of the stack of cards in
	the selected 2D scene.

	The vertices and colors are defined in the shapes namespace as functions that return OpenGLMesh objects.
	*/

	// define shape to send to scene
	OpenGLMesh bottomCards = shapes::Cube();
	bottomCards.setScale(1.6f, 0.4f, 1.4f);
	bottomCards.setRotation(45.0f, 0.0f, 1.0f, 0.0f); // normals are rotated at render, vertices here
	bottomCards.setTranslation(-2.0f, 0.0f, 0.0f);
	bottomCards.texture = "textures/vegas-deck.png";
	// add to scene
	GLControl.AddShape(bottomCards); 
	
	OpenGLMesh topCards = shapes::Cube();
	topCards.setScale(1.6f, 0.4f, 1.4f);
	topCards.setRotation(44.9f, 0.0f, 1.0f, 0.0f);
	topCards.setTranslation(-2.0f, 0.4f, 0.0f);
	topCards.texture = "textures/scooby-deck.png";
	GLControl.AddShape(topCards);

	/*
	* Create one plane that will serve to ground the scene
	*/

	OpenGLMesh basePlane = shapes::Plane();
	basePlane.setScale(8.0f, 8.0f, 8.0f);
	basePlane.setRotation(0.0f, 1.0f, 0.0f, 0.0f);
	basePlane.setTranslation(-4.0f, 0.0f, 2.0f);
	basePlane.texture = "textures/wood.jpg";
	GLControl.AddShape(basePlane); // consider pass by reference!

	/*
	* Create cylinders that will represent the shape of a candle
	*/

	OpenGLMesh CandleBottom = shapes::Cylinder(64); // a 64-sided block well approximates a cylinder
	CandleBottom.setScale(1.4f, 1.8f, 1.4f);
	CandleBottom.setRotation(3.14f, 0.0f, 1.0f, 0.0f);
	CandleBottom.setTranslation(2.0f, 0.0f, -1.5f);
	CandleBottom.texture = "textures/candle.jpg"; 
	GLControl.AddShape(CandleBottom);

	OpenGLMesh CandleTop = shapes::Cylinder(64);
	CandleTop.setScale(1.45f, 0.3f, 1.45f);
	CandleTop.setRotation(3.14f, 0.0f, 1.0f, 0.0f);
	CandleTop.setTranslation(2.0f, 1.8f, -1.5f);
	CandleTop.texture = "textures/lidside.png"; //lidside2.png  
	GLControl.AddShape(CandleTop);

	OpenGLMesh CandleTop2 = shapes::Cylinder(64);
	CandleTop2.setScale(1.45f, 0.01f, 1.45f);
	CandleTop2.setRotation(3.14f, 0.0f, 1.0f, 0.0f);
	CandleTop2.setTranslation(2.0f, 2.1f, -1.5f);
	CandleTop2.texture = "textures/metal.jpg";    //metal.jpg
	GLControl.AddShape(CandleTop2);

	/*
	* Create the single rubber ball
	*/

	OpenGLMesh ball = shapes::Sphere(32, 32);
	ball.setScale(0.6f, 0.6f, 0.6f);
	ball.setRotation(0.0f, 1.0f, 0.0f, 0.0f); 
	ball.setTranslation(-0.25f, 0.58f, 0.25f);
	ball.texture = "textures/bluerubber.jpg"; 
	ball.setShininess(0.25f); // 0 - 1 with 1 being quite shiny
	GLControl.AddShape(ball);

	/*
	* and now, 'the duck'
	*/
	const char* duckTexture = "textures/blue.png";
	float duckShinyness = 0.25f;

	auto body = shapes::Sphere(16, 16);
	body.setScale(0.7f, 0.5f, 0.8f);
	body.setRotation(0, 1, 0, 0);
	body.setTranslation(-2.0f, 1.05, -1.0f);
	body.texture = duckTexture;
	body.setShininess(duckShinyness);
	GLControl.AddShape(body);


	auto lWing = shapes::Sphere(16, 16);
	lWing.setScale(0.1f, 0.25f, 0.33f);
	lWing.setRotation(0.2f, 1, 0, 0);
	lWing.setTranslation(-2.65f, 1.05, -1.0f);
	lWing.texture = duckTexture;
	lWing.setShininess(duckShinyness);
	GLControl.AddShape(lWing);


	auto rWing = shapes::Sphere(16, 16);
	rWing.setScale(0.1f, 0.25f, 0.33f);
	rWing.setRotation(0.2f, 1, 0, 0);
	rWing.setTranslation(-1.35f, 1.05, -1.0f);
	rWing.texture = duckTexture;
	rWing.setShininess(duckShinyness);
	GLControl.AddShape(rWing);


	auto tail = shapes::Sphere(32, 32);
	tail.setScale(0.4f, 0.15f, 0.35f);
	tail.setRotation(1.0f, 1, 0, 0);
	tail.setTranslation(-2.0f, 1.05, -1.7f);
	tail.texture = duckTexture;
	tail.setShininess(duckShinyness);
	GLControl.AddShape(tail);


	auto head = shapes::Sphere(16, 16);
	head.setScale(0.4f, 0.45f, 0.45f);
	head.setRotation(0, 1, 0, 0);
	head.setTranslation(-2.0f, 1.85f, -.75f);
	head.texture = duckTexture;
	head.setShininess(duckShinyness);
	GLControl.AddShape(head);


	auto lips = shapes::Sphere(32, 32);
	lips.setScale(0.3f, 0.12f, 0.4f);
	lips.setRotation(0, 1, 0, 0);
	lips.setTranslation(-2.0f, 1.60f, -0.5f);
	lips.texture = "textures/orange.jpg";
	GLControl.AddShape(lips);


	auto lipsTop = shapes::Sphere(32, 32);
	lipsTop.setScale(0.2f, 0.18f, 0.2f);
	lipsTop.setRotation(0, 1, 0, 0);
	lipsTop.setTranslation(-2.0f, 1.67f, -0.4f);
	lipsTop.texture = "textures/orange.jpg";
	GLControl.AddShape(lipsTop);


	auto lEye = shapes::Sphere(16, 16);
	lEye.setScale(0.05f, 0.1f, 0.1f);
	lEye.setRotation(0.4f, 0, 1, 0);
	lEye.addRotation(3.14f, 1, 0, 0);
	lEye.setTranslation(-2.3f, 1.87f, -0.5f);
	lEye.texture = "textures/eye.png";
	GLControl.AddShape(lEye);

	
	auto rEye = shapes::Sphere(16, 16);
	rEye.setScale(0.05f, 0.1f, 0.1f);
	rEye.setRotation(-0.4f, 0, 1, 0);
	rEye.addRotation(3.14f, 0, 1, 0);
	rEye.addRotation(3.14f, 1, 0, 0);
	rEye.setTranslation(-1.7f, 1.87f, -0.5f);
	rEye.texture = "textures/eye.png";
	GLControl.AddShape(rEye);
	
	/*
	* Set starting view and projection parameters in the controller's main camera
	*/
	GLControl.MainCamera.setPosition(0.0f, 1.5f, 5.0f);
	GLControl.MainCamera.setFieldOfView(45.0f);
	

	/*
	* Set lighting by adding lights to the scene and setting parameters directly
	* 
	*/

	Light mainLight;
	//mainLight.setColor(1.0f, 0.976f, 0.992f); // 6500K (cool)
	//mainLight.setColor(1.0f, 0.706f, 0.4196f); // 3000K (warm)
	//mainLight.setColor(1.0f, 0.859f, 0.729f); // 4500K (neutral) 
	mainLight.setColor(1.0f, 0.894f, 0.807f); // 5000K (coolish) (matches my living room)
	mainLight.setIntensity(1.0f);
	mainLight.setPosition(5.0f, 5.0f, -5.0f);
	GLControl.AddLight(mainLight);

	Light backLight;
	//backLight.setColor(1.0f, 0.976f, 0.992f); // 6500K (cool)
	//backLight.setColor(1.0f, 0.706f, 0.4196f); // 3000K (warm)
	//backLight.setColor(1.0f, 0.859f, 0.729f); // 4500K (neutral) 
	backLight.setColor(1.0f, 0.894f, 0.807f); // 5000K (coolish)  
	backLight.setIntensity(1.0f);
	backLight.setPosition(-5.0f, 5.0f, 5.0f);
	GLControl.AddLight(backLight);


	// render loop
	GLControl.RunScene();
	return 0;
}
