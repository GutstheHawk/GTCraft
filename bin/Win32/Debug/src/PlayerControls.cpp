#include "PlayerControls.h"

PlayerControls::PlayerControls(Camera* cam, Superchunk* supChunk)
{
	camera = cam;
	sChunk = supChunk;
}

void PlayerControls::ProcessInputs()
{
	//Main Keys
	if (W_PRESSED)
		//printf("You pressed the w key!\n");
		camera->MoveForward();

	if (A_PRESSED)
		//printf("You pressed the a key!\n");
		camera->StrafeLeft();

	if (S_PRESSED)
		//printf("You pressed the s key!\n");
		camera->MoveBackward();

	if (D_PRESSED)
		//printf("You pressed the d key!\n");
		camera->StrafeRight();

	if (SPACE_PRESSED)
		camera->MoveUpward();

	if (LEFT_SHIFT_PRESSED)
		camera->MoveDownward();

	if (UP_PRESSED)
		camera->Rotate(glm::vec2(0.0f, 0.05f));

	if (LEFT_PRESSED)
		camera->Rotate(glm::vec2(-0.05f, 0.0f));

	if (DOWN_PRESSED)
		camera->Rotate(glm::vec2(0.0f, -0.05f));

	if (RIGHT_PRESSED)
		camera->Rotate(glm::vec2(0.05f, 0.0f));

	//Mouse Input

	if (LEFT_MOUSE_PRESSED)
	{
		leftMousePresses++;
		//camera->CastRay(4.0f);
		if (leftMousePresses == 1)
		{
			sChunk->breakBlock(camera);
		}
	}

	if (RIGHT_MOUSE_PRESSED)
	{
		rightMousePresses++;
		//camera->CastRay(4.0f);
		if (rightMousePresses == 1)
		{
			sChunk->breakBlock(camera);
		}
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera->SetCameraFront(glm::normalize(front));

	camera->SetCameraRight();
	camera->SetCameraUp();

	camera->UpdateView();
}