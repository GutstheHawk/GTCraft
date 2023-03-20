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

	PlayerControls(Camera*);
	~PlayerControls();
	void ProcessInputs() const;
};

