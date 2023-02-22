#pragma once

#include <GLFW/glfw3.h>
#include <iostream>



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //Mouse Movement


    //Main Keys
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        printf("You pressed the w key!\n");

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        printf("You pressed the a key!\n");

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        printf("You pressed the s key!\n");

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        printf("You pressed the d key!\n");

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
        }
        else
        {
            glfwSetWindowMonitor(window, NULL, 200, 200, 1280, 720, 0);
        }
    }
           
}