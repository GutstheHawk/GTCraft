#include "PlayerControls.h"

PlayerControls::PlayerControls(Camera* cam)
{
	camera = cam;
}

void PlayerControls::ProcessInputs() const
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
}