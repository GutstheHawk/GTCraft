#pragma once

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <string>

#include "Superchunk.h"

static int charArrayToInt(char* chars)
{
	int outputInt = 0;

	for (int i = 0; chars[i] != '\0'; i++)
	{
		if (isdigit(chars[i]))
		{
			outputInt += static_cast<int>(chars[i]) - static_cast<int>('0');
		}
		else
		{
			outputInt += static_cast<int>(chars[i]);
		}
	}

	return outputInt;
}

static void showMainMenu(GLFWwindow* window, Superchunk* sChunk, bool* startMenuToggle, char* seed)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	int windowWidth, windowHeight;
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	int menuWidth = windowWidth - (windowWidth / 4);
	int menuHeight = windowHeight - (windowHeight / 4);

	ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight));
	ImGui::SetNextWindowPos(ImVec2((windowWidth / 2) - (menuWidth / 2), (windowHeight / 2) - (menuHeight / 2)));

	ImGui::Begin("Main Menu");

	ImGui::InputText("World Seed", seed, 64);

	if (ImGui::Button("Generate World"))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		sChunk->setSeed(charArrayToInt(seed));
		sChunk->generateSuperchunk();
		*startMenuToggle = false;
	}

	ImGui::Spacing();
	ImGui::Text("Controls");
	ImGui::Spacing();
	ImGui::Text("- Move Player: WASD keys and mouse movement");
	ImGui::Text("- Move Up and Down: Space and Shift keys");
	ImGui::Text("- Remove Block: Left Click");
	ImGui::Text("- Place Block: Right Click");
	ImGui::Text("- Open and Close Inventory: E key");
	ImGui::Text("- Quick Select Blocks: 0-9 keys");
	ImGui::Spacing();
	ImGui::Text("- Fullscreen: F1");
	ImGui::Text("- Exit Program: ESC");

	ImGui::End();
}