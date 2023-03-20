#include "Camera.h"

Camera::Camera()
	: Projection(glm::perspective(95.0f, 16.0f / 9.0f, 0.1f, 100.f)),
	View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))),
	Model(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)))
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
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::ReturnPVM()
{
	return Projection * View * Model;
}

