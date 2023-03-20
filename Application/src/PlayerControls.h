#pragma once
#include "Camera.h"

class PlayerControls
{
public:
	Camera* camera;

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

	float pitch = 0.0f;
	float yaw = 0.0f;

	float xpos = 0.0f;
	float ypos = 0.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;

	PlayerControls(Camera*);
	~PlayerControls();
	void ProcessInputs();
};

