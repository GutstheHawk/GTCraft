#pragma once

#include "imgui/imgui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "BlockDefinitions.h"
#include "PlayerControls.h"

#include <utility>
#include <array>
#include <string>

static std::array<std::pair<std::string, uint8_t>, 32> Blocks = {
        std::make_pair("Grass", GRASS),
        std::make_pair("Dirt", DIRT),
        std::make_pair("Stone", STONE),
        std::make_pair("Cobblestone", COBBLESTONE),
        std::make_pair("Oak Log", OAKLOG),
        std::make_pair("Oak Plank", OAKPLANK),
        std::make_pair("Gravel", GRAVEL),
        std::make_pair("Sand", SAND),

        std::make_pair("Soul Sand", SOULSAND),
        std::make_pair("Stone Brick", STONEBRICK),
        std::make_pair("Red Brick", REDBRICK),
        std::make_pair("Granite", GRANITE),
        std::make_pair("Terracotta", TERRACOTTA),
        std::make_pair("Slime", SLIME),
        //std::make_pair("Cactus", CACTUSTOP),
        std::make_pair("Jack O Lantern", PUMPKINTOP),
        std::make_pair("Melon", MELONSIDE),

        std::make_pair("Lapis Block", LAPISBLOCK),
        std::make_pair("Black Wool", BLACKWOOL),
        std::make_pair("Dark Grey Wool", DARKGREYWOOL),
        std::make_pair("Red Wool", REDWOOL),
        std::make_pair("Pink Wool", PINKWOOL),
        std::make_pair("Green Wool", GREENWOOL),
        std::make_pair("Light Green Wool", LIGHTGREENWOOL),
        std::make_pair("Brown Wool", BROWNWOOL),

        std::make_pair("Yellow Wool", YELLOWWOOL),
        std::make_pair("Blue Wool", BLUEWOOL),
        std::make_pair("Light Blue Wool", LIGHTBLUEWOOL),
        std::make_pair("Purple Wool", PURPLEWOOL),
        std::make_pair("Magenta Wool", MAGENTAWOOL),
        std::make_pair("Teal Wool", TEALWOOL),
        std::make_pair("Orange Wool", ORANGEWOOL),
        std::make_pair("Grey Wool", GREYWOOL)

};

static void renderBlockSelection(PlayerControls* pc)
{
    int selected_index = -1; // Set initially to an invalid index

    ImGui::BeginTable("Blocks", 4);

    for (int i = 0; i < 8; i++)
    {
        ImGui::TableNextRow();

        for (int j = 0; j < 4; j++)
        {
            int index = i * 4 + j;

            // Check if this item is selected
            bool is_selected = (index == selected_index);

            // Begin the cell
            ImGui::TableNextColumn();

            // Set the button color based on the selection status
            if (is_selected)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            }

            // Create the button
            if (index < Blocks.size())
            {
                if (ImGui::Button(Blocks[index].first.c_str()))
                {
                    // Set the selected index when the button is clicked
                    pc->selectedBlockType = Blocks[index].second;
                    selected_index = index;
                }
            }

            // End the cell
            ImGui::TableNextColumn();

            // Reset the button color if necessary
            if (is_selected)
            {
                ImGui::PopStyleColor(3);
            }
        }
    }

    ImGui::EndTable();
}