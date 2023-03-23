#include "Camera.h"

Camera::Camera()
	: Projection(glm::perspective(glm::radians(95.0f), 16.0f / 9.0f, 0.1f, 100.f)),
	View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))),
	Model(glm::mat4(1.0f)), cameraRight(glm::vec3(0.0f, 0.0f, 0.0f)), cameraUp(glm::vec3(0.0f, 0.0f, 0.0f))
	//glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))
{
}

Camera::~Camera()
{
}

void Camera::ResetModel()
{
	Model = glm::mat4(1.0f);
}

void Camera::Rotate(glm::vec2 const& Rotate)
{
	Model = glm::rotate(Model, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ModelTransform(glm::vec3 translationVector)
{
	glm::mat4 Trans = glm::mat4(1.0f);
	Trans = glm::translate(Trans, translationVector);
	Model = Trans;
}

// Set camera parameters
void Camera::SetCameraSpeed(float deltaTime)
{
	cameraSpeed = 8 * deltaTime;
}

void Camera::SetCameraFront(glm::vec3 camFront)
{
	cameraFront = camFront;
}

void Camera::SetCameraRight()
{
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
}

void Camera::SetCameraUp()
{
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::UpdateView()
{
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::ReturnUpdatedView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}


//Player movement via camera transforms and rotations
void Camera::MoveForward()
{
	cameraPos += cameraSpeed * cameraFront;
	UpdateView();
}

void Camera::MoveBackward()
{
	cameraPos -= cameraSpeed * cameraFront;
}

void Camera::StrafeLeft()
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::StrafeRight()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::MoveUpward()
{
	cameraPos += cameraSpeed * cameraUp;
}

void Camera::MoveDownward()
{
	cameraPos += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
}



//Camera get methods
glm::mat4 Camera::ReturnProjection()
{
	return Projection;
}

glm::mat4 Camera::ReturnView()
{
	return View;
}

glm::mat4 Camera::ReturnModel()
{
	return Model;
}

glm::mat4 Camera::ReturnPVM()
{
	return Projection * View * Model;
}


