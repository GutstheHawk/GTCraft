#pragma once

#include "imgui/imgui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "BlockDefinitions.h"
#include "PlayerControls.h"

#include <utility>
#include <array>
#include <string>

static std::array<std::pair<std::string, uint8_t>, 3> Blocks = {
        std::make_pair("Grass", GRASS),
        std::make_pair("Dirt", DIRT),
        std::make_pair("Stone", STONE)
};

static void renderBlockSelection(PlayerControls* pc)
{

    int selected_index = -1; // Set initially to an invalid index

    ImGui::BeginTable("Blocks", 1);

    for (int i = 0; i < 3; i++) {
        // Check if this item is selected
        bool is_selected = (i == selected_index);

        // Begin the cell
        ImGui::TableNextColumn();

        // Set the button color based on the selection status
        if (is_selected)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        // Create the buttons
        //for (const auto& pair : Blocks) {
        //    if (ImGui::Button(pair.first.c_str())) {
        //        // Set the selected index when the button is clicked
        //        pc->selectedBlockType = pair.second;
        //        selected_index = i;
        //    }
        //}

        if (ImGui::Button(Blocks[i].first.c_str()))
        {
            // Set the selected index when the button is clicked
            pc->selectedBlockType = Blocks[i].second;
            selected_index = i;
        }

        // End the cell
        ImGui::TableNextColumn();

        // Reset the button color if necessary
        if (is_selected)
        {
            ImGui::PopStyleColor(3);
        }
    }

    ImGui::EndTable();

}