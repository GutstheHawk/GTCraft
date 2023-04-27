#include "Camera.h"

Camera::Camera()
	: Projection(glm::perspective(glm::radians(95.0f), 16.0f / 9.0f, 0.1f, 150.f)),
	View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))),
	Model(glm::mat4(1.0f)), cameraRight(glm::vec3(0.0f, 0.0f, 0.0f)), cameraUp(glm::vec3(0.0f, 0.0f, 0.0f))
	//glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))
{
	yaw = 0.0f;
	pitch = 0.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	SetCameraFront(glm::normalize(front));

	SetCameraRight();
	SetCameraUp();
	UpdateView();
}

Camera::~Camera()
{
}

std::pair<glm::vec3, glm::vec3> Camera::CastRay(float length)
{
	// Get the dimensions of the viewport
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Compute the center of the screen
	float x = viewport[2] / 2.0f;
	float y = viewport[3] / 2.0f;

	// Compute the direction of the ray
	glm::vec3 rayDirection = glm::normalize(cameraFront);

	// Compute the starting point of the ray
	glm::vec3 rayOrigin = cameraPos + rayDirection;

	// Cast the ray
	glm::vec3 rayEnd = rayOrigin + rayDirection * length;

	//RenderRay(rayOrigin, rayEnd);
	//std::cout << "Starting Pos: " << to_string(rayOrigin) << std::endl;
	//std::cout << "Ending Pos: " << to_string(rayEnd) << std::endl;

	std::pair<glm::vec3, glm::vec3> result = TwoVec3s(rayOrigin, rayEnd);

	return result;
}

void Camera::RenderRay(glm::vec3 rayOrigin, glm::vec3 rayEnd)
{
	worldShader->Unbind();
	rayShader->Bind();

	rayShader->SetUniformMatrix4fv("projection", 1, GL_FALSE, &Projection[0][0]);
	rayShader->SetUniformMatrix4fv("view", 1, GL_FALSE, &View[0][0]);

	GLfloat vertices[] = {
		rayOrigin.x, rayOrigin.y, rayOrigin.z,
		rayEnd.x, rayEnd.y, rayEnd.z
	};
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	rayShader->Unbind();
	worldShader->Bind();
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
	cameraPos += cameraSpeed * worldUp;
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


