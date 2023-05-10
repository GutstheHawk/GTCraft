#include "Camera.h"

Camera::Camera()
{
	FOV = 95.0f;
	nearClip = 0.1f;
	farClip = 150.0f;

	Projection = glm::perspective(glm::radians(FOV), 16.0f / 9.0f, nearClip, farClip);
	View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	Model = glm::mat4(1.0f);

	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3{ 0.0f, 1.0f, 0.0f };
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = 0.0f;
	pitch = 0.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	SetCameraFront(glm::normalize(front));

	SetCameraRight();
	UpdateCameraUp();
	UpdateView();
}

std::pair<glm::vec3, glm::vec3> Camera::CastRay(float length)
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float x = viewport[2] / 2.0f;
	float y = viewport[3] / 2.0f;

	glm::vec3 rayDirection = glm::normalize(cameraFront);

	glm::vec3 rayOrigin = cameraPos + rayDirection;

	glm::vec3 rayEnd = rayOrigin + rayDirection * length;


	std::pair<glm::vec3, glm::vec3> result = TwoVec3s(rayOrigin, rayEnd);

	return result;
}

void Camera::SetWorldShader(Shader* inWorldShader)
{
	worldShader = inWorldShader;
}

void Camera::SetRayShader(Shader* inRayShader)
{
	rayShader = inRayShader;
}


void Camera::Teleport(glm::vec3 position)
{
	cameraPos = position;
	UpdateView();
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
void Camera::SetCameraPosition(glm::vec3 camPos)
{
	cameraPos = camPos;
}

void Camera::SetCameraFront(glm::vec3 camFront)
{
	cameraFront = camFront;
}

void Camera::SetCameraUp(glm::vec3 camUp)
{
	cameraUp = camUp;
}

void Camera::SetCameraSpeed(float deltaTime)
{
	cameraSpeed = 8 * deltaTime;
}

void Camera::SetCameraRight()
{
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::UpdateCameraUp()
{
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::UpdateView()
{
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
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
	cameraPos -= glm::normalize(glm::cross(cameraFront, worldUp)) * cameraSpeed;
}

void Camera::StrafeRight()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, worldUp)) * cameraSpeed;
}

void Camera::MoveUpward()
{
	cameraPos += cameraSpeed * worldUp;
}

void Camera::MoveDownward()
{
	cameraPos += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
}



//Camera get methods
glm::mat4 Camera::GetProjection()
{
	return Projection;
}

glm::mat4 Camera::GetView()
{
	return View;
}

glm::mat4 Camera::GetModel()
{
	return Model;
}

glm::mat4 Camera::GetPVM()
{
	return Projection * View * Model;
}

glm::vec3 Camera::GetCameraPosition()
{
	return cameraPos;
}

glm::vec3 Camera::GetCameraFront()
{
	return cameraFront;
}

glm::vec3 Camera::GetCameraUp()
{
	return cameraUp;
}

