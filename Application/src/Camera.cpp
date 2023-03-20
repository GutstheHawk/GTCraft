#include "Camera.h"

Camera::Camera()
	: Projection(glm::perspective(glm::radians(95.0f), 16.0f / 9.0f, 0.1f, 100.f)),
	View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))),
	Model(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
{
}

Camera::~Camera()
{
}

void Camera::ViewTranslate(float Translate)
{
	View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
}

void Camera::Rotate(glm::vec2 const& Rotate)
{
	Model = glm::rotate(Model, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::SetCameraSpeed(float deltaTime)
{
	cameraSpeed = 5 * deltaTime;
}


void Camera::MoveForward()
{
	cameraPos += cameraSpeed * cameraFront;
	UpdateView();
}

void Camera::MoveBackward()
{
	cameraPos -= cameraSpeed * cameraFront;
	UpdateView();
}

void Camera::StrafeLeft()
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	UpdateView();
}

void Camera::StrafeRight()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	UpdateView();
}

void Camera::MoveUpward()
{
	cameraPos += cameraSpeed * cameraUp;
	UpdateView();
}

void Camera::MoveDownward()
{
	cameraPos += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
	UpdateView();
}


void Camera::UpdateView()
{
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}


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


