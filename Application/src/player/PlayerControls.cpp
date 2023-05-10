#include "PlayerControls.h"

PlayerControls::PlayerControls(Camera* cam)
{
	camera = cam;
}

void PlayerControls::ProcessInputs()
{
	//Main Keys
	if (W_PRESSED)
		camera->MoveForward();

	if (A_PRESSED)
		camera->StrafeLeft();

	if (S_PRESSED)
		camera->MoveBackward();

	if (D_PRESSED)
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
		if (leftMousePresses == 1 && !inventoryToggle)
		{
			sChunk->breakBlock(camera);
		}
	}

	if (RIGHT_MOUSE_PRESSED)
	{
		rightMousePresses++;
		//camera->CastRay(4.0f);
		if (rightMousePresses == 1 && !inventoryToggle)
		{
			sChunk->placeBlock(camera, selectedBlockType);
		}
	}

	if (saveState)
	{
		saveState = false;
		//saveWorld(*sChunk, "saves/world.bin");
	}

	if (loadState)
	{
		loadState = false;
		//sChunk->~Superchunk();
		//sChunk = loadWorld("saves/world.bin");
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

	camera->UpdateCameraRight();
	camera->UpdateCameraUp();

	camera->UpdateView();
}