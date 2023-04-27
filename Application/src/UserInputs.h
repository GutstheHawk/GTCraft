#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "PlayerControls.h"
//#include "Camera.h"


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    //Camera* cam = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
	PlayerControls* pc = reinterpret_cast<PlayerControls*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		if (pc->inventoryToggle == true)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			pc->inventoryToggle = false;

		}
		else if (pc->inventoryToggle == false)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			pc->inventoryToggle = true;
		}

	}


    //Main Keys
    //if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    //    //printf("You pressed the w key!\n");
    //    cam->MoveForward();
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		//printf("You pressed the a key!\n");
		pc->W_PRESSED = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		pc->W_PRESSED = false;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        //printf("You pressed the a key!\n");
        pc->A_PRESSED = true;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		pc->A_PRESSED = false;

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        //printf("You pressed the s key!\n");
		pc->S_PRESSED = true;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		pc->S_PRESSED = false;

    if (key == GLFW_KEY_D && action == action == GLFW_PRESS)
        //printf("You pressed the d key!\n");
		pc->D_PRESSED = true;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		pc->D_PRESSED = false;

	if (key == GLFW_KEY_SPACE && action == action == GLFW_PRESS)
		pc->SPACE_PRESSED = true;
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		pc->SPACE_PRESSED = false;

	if (key == GLFW_KEY_LEFT_SHIFT && action == action == GLFW_PRESS)
        pc->LEFT_SHIFT_PRESSED = true;
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		pc->LEFT_SHIFT_PRESSED = false;

    //Function Keys
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        GLFWmonitor* monitor = glfwGetWindowMonitor(window);
        if (monitor == NULL)
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			//glfwSetWindowMonitor(window, primaryMonitor, 0, 0, 1920, 1080, mode->refreshRate);
        }
        else
        {
			glfwSetWindowMonitor(window, NULL, 200, 200, 1280, 720, 0);
            //glfwSetWindowMonitor(window, NULL, 200, 200, 1920, 1080, 0);
        }
    }

	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		pc->saveState = true;
	}

	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		pc->loadState = true;
	}

	//Object Rotation Keys
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        pc->UP_PRESSED = true;
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		pc->UP_PRESSED = false;
	
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        pc->LEFT_PRESSED = true;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		pc->LEFT_PRESSED = false;

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		pc->DOWN_PRESSED = true;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		pc->DOWN_PRESSED = false;

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		pc->RIGHT_PRESSED = true;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		pc->RIGHT_PRESSED = false;

}

void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	PlayerControls* pc = reinterpret_cast<PlayerControls*>(glfwGetWindowUserPointer(window));
	
	if (pc->inventoryToggle == false)
	{
		pc->xpos = xpos;
		pc->ypos = ypos;
	}

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	PlayerControls* pc = reinterpret_cast<PlayerControls*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		pc->LEFT_MOUSE_PRESSED = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		pc->LEFT_MOUSE_PRESSED = false;
		pc->leftMousePresses = 0;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		pc->RIGHT_MOUSE_PRESSED = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		pc->RIGHT_MOUSE_PRESSED = false;
		pc->rightMousePresses = 0;
	}



}