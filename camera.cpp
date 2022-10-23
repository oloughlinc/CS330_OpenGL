#include "camera.h"

/* Set the default view and position */
Camera::Camera() {

	// default speed is at half of full scale
	mPanSpeed = PAN_SPEED_RANGE / 2;
	mRotateSpeed = ROTATE_SPEED_RANGE / 2;

	// default position, pointing at center along z axis
	mCameraPosition = CENTER;
	mCameraPointsAt = -Z_AXIS;
	mCameraHorizontalAxis = X_AXIS;
	mCameraVerticalAxis = Y_AXIS;

	// default rotation (these dont apply until rotate is called)
	mRotationX = 1.5f * 3.14159f;
	mRotationY = 0;

	// default FOV
	mFieldOfView = 45.0f;
}

void Camera::setPosition(float x, float y, float z) {
	mCameraPosition = glm::vec3(x, y, z);
}

void Camera::setFieldOfView(float angle) {
	mFieldOfView = angle;
}

float Camera::getFieldOfView() {
	return mFieldOfView;
}

glm::mat4 Camera::getView() {
	return glm::lookAt(mCameraPosition, mCameraPosition + mCameraPointsAt, Y_AXIS);
}

void Camera::move(CameraDirection direction, float frameTime) {

	// modulate movement speed by frame render speed, faster frames mean less movement per frame
	float movementSpeed = mPanSpeed * frameTime;

	switch (direction) {

		case CameraDirection::NONE:
			break;

	    case CameraDirection::FORWARD:
			mCameraPosition += mCameraPointsAt * movementSpeed;
			break;

		case CameraDirection::BACK:
			mCameraPosition -= mCameraPointsAt * movementSpeed;
			break;

		case CameraDirection::LEFT:
			mCameraPosition -= mCameraHorizontalAxis * movementSpeed;
			break;

		case CameraDirection::RIGHT:
			mCameraPosition += mCameraHorizontalAxis * movementSpeed;
			break;

		case CameraDirection::UP:
			mCameraPosition += mCameraVerticalAxis * movementSpeed;
			break;

		case CameraDirection::DOWN:
			mCameraPosition -= mCameraVerticalAxis * movementSpeed;
			break;

		default:
			break;
	}
}

void Camera::rotate(float x, float y) {
	
	// modulate raw rotation delta by speed
	mRotationX += x * mRotateSpeed;
	mRotationY += y * mRotateSpeed;

	// constrain up/down rotation to 180 degrees
	mRotationY = std::clamp(mRotationY, -0.5f * 3.14159f, 0.5f * 3.14159f);

	// polar angle to rectangular coordinate on a sphere
	auto xCircle = glm::vec3(cos(mRotationX), 1.0f, sin(mRotationX));
	auto yCircle = glm::vec3(cos(mRotationY), sin(mRotationY), cos(mRotationY));
	mCameraPointsAt = glm::normalize(xCircle * yCircle); // camera aims at this point in space, which is on a unit sphere

	// align the vertical and horizontal axes with the new camera heading
	mCameraHorizontalAxis = glm::normalize(glm::cross(mCameraPointsAt, Y_AXIS)); //align camera left right pan to new axis
	mCameraVerticalAxis = glm::normalize(glm::cross(mCameraHorizontalAxis, mCameraPointsAt));
	
}

void Camera::changeMovementSpeed(float scale) {

	// scale is 1 or -1 from GLFW
	mPanSpeed += (PAN_SPEED_RANGE / STEP_SIZE) * scale;
	mRotateSpeed += (ROTATE_SPEED_RANGE / STEP_SIZE) * scale;

	mPanSpeed = std::clamp(mPanSpeed, PAN_SPEED_MIN, PAN_SPEED_MAX);
	mRotateSpeed = std::clamp(mRotateSpeed, ROTATE_SPEED_MIN, ROTATE_SPEED_MAX);
}