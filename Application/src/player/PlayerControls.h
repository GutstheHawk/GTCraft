#pragma once
#include "Camera.h"

class PlayerControls
{
public:
	Camera* camera;
<<<<<<< Updated upstream:Application/src/PlayerControls.h
=======
	Superchunk* sChunk;

	uint8_t selectedBlockType = DIRT;

	glm::vec3 cachedCameraPos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cachedCameraFront = { 0.0f, 0.0f, 0.0f };
	glm::vec3 cachedCameraUp = { 0.0f, 0.0f, 0.0f };

	bool saveState = 0;
	bool loadState = 0;

	bool inventoryToggle = false;
>>>>>>> Stashed changes:Application/src/player/PlayerControls.h

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

	float pitch = -90.0f;
	float yaw = 0.0f;

	float xpos = 0.0f;
	float ypos = 0.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;

<<<<<<< Updated upstream:Application/src/PlayerControls.h
	PlayerControls(Camera*);
	~PlayerControls();
=======
	PlayerControls(Camera*, Superchunk* sChunk);
>>>>>>> Stashed changes:Application/src/player/PlayerControls.h
	void ProcessInputs();
};

