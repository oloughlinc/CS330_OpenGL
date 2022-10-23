#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm> //clamp

enum class CameraDirection {
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
};


class Camera {

	/* A representation of view in the 3D scene as a transformable object. The current view built by this object will be 
	queried by the render algorithm at runtime. The view is a matrix representation in a model-view-projection scheme.
	
	The camera is meant to be controlled for pan and rotation from GLFW keyboard/mouse input.
	*/

	// access full functionality of the camera from OpenGL controller 
    // not intended to be controlled from main
	friend class OpenGL;

	public:

		Camera();

		void setPosition(float x, float y, float z);
		void setFieldOfView(float angle);

	private:

	#define CENTER glm::vec3(0,0,0)
	#define X_AXIS glm::vec3(1,0,0)
	#define Y_AXIS glm::vec3(0,1,0)
	#define Z_AXIS glm::vec3(0,0,1)

		// define camera scene position information
		glm::vec3 mCameraPointsAt; // "front", where the camera aims. front back control moves on this axis
		glm::vec3 mCameraPosition; // transform position in world space
		glm::vec3 mCameraHorizontalAxis; // left right control moves on this axis, which must change with rotation
		glm::vec3 mCameraVerticalAxis; // up down moves on this axis, w.m.c.w.r.
		float mRotationX; // camera horizontal rotation (to world space)
		float mRotationY; // camera vertical rotation (tws)

		// define movement speed bounds
		const float PAN_SPEED_MAX = 10.0f;
		const float PAN_SPEED_MIN = 1.0f;
		const float ROTATE_SPEED_MAX = 0.01f;
		const float ROTATE_SPEED_MIN = 0.0005f;
		const int STEP_SIZE = 20;

		// define range of speed
		const float PAN_SPEED_RANGE = PAN_SPEED_MAX - PAN_SPEED_MIN;
		const float ROTATE_SPEED_RANGE = ROTATE_SPEED_MAX - ROTATE_SPEED_MIN;

		// define active movement speed, variable based on input
		float mPanSpeed;
		float mRotateSpeed;

		// define field of view for projection
		float mFieldOfView;

		// define primary functions
		glm::mat4 getView();

		void rotate(float x, float y);
		void move(CameraDirection direction, float frameTime);
		void changeMovementSpeed(float scale);
		float getFieldOfView();

};


#endif
