#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        printf("You pressed the e key!\n");
}