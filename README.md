# GTCraft - A Minecraft-Style Creative Game

![alt text](https://github.com/GutstheHawk/GTCraft/blob/main/Application/res/screenshots/GTCraft.png?raw=true)

A game I chose to create for my senior capstone project for my undergrad degree in Computer Science. It was developed as a solo venture over the course of about 3 months.

This build of the game was designed to roughly replicate the limited sandbox environment of Minecraft Classic.
You as the player are able to explore a randomly generated world and place blocks to your liking, much like the early version of the now world-renowned game.

**The game currently features:**
- FPS-style first person camera controls
- A limited size gameworld randomly generated using GLM's Perlin Noise implementation
- Block placing and breaking
- A palette of 32 different blocks accessible to the player through an in-game GUI
- Randomly generated trees, foliage, and water to populate the world

**Features soon to be added:**
- Saving and loading of the gameworld
- Player physics and collision detection
- Adaptive lighting depending on time of day and light source placement
- Chunk caching in saved files and infinite world generation

**Technologies Used:**
- C++ with Visual Studio
- OpenGL with GLFW
- GLM math library
- imGUI
- Cereal

**How To Play:**\
Simply download the main branch to your PC and launch the Application.exe file inside of the Application folder

**Credits:**\
Main Developer - Brandon Molitor\
All included libraries use the MIT license and are OK for distribution.\
Texture Atlas by Rhodox - Painterly Pack - https://resourcepack.net/painterly-resource-pack/#gsc.tab=0 \
Skybox Texture by Finniespin - https://garrysmods.org/download/12004/skybox-minecraft
