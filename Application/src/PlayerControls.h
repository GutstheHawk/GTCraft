#pragma once
#include "Camera.h"
#include "Superchunk.h"

class PlayerControls
{
public:
	Camera* camera;
	Superchunk* sChunk;

	bool W_PRESSED = false;
	bool A_PRESSED = false;
	bool S_PRESSED = false;
	bool D_PRESSED = false;
	bool SPACE_PRESSED = false;
	bool LEFT_SHIFT_PRESSED = false;

	bool UP_PRESSED;
	bool DOWN_PRESSED;
	bool LEFT_PRESSED;
	bool RIGHT_PRESSED;

	bool LEFT_MOUSE_PRESSED = false;
	unsigned int leftMousePresses = 0;
	bool RIGHT_MOUSE_PRESSED = false;
	unsigned int rightMousePresses = 0;

	float pitch = -90.0f;
	float yaw = 0.0f;

	float xpos = 0.0f;
	float ypos = 0.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;

	PlayerControls(Camera*, Superchunk* sChunk);
	~PlayerControls();
	void ProcessInputs();
};

