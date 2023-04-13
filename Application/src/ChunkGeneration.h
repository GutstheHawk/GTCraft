#pragma once

#include <iostream>
#include <fstream>
#include <string>

struct Block {
	unsigned char BlockID;
	unsigned char ZX;
	unsigned short Y;
};

static Block* generateChunk()
{
	Block chunk[4096];

	unsigned char localBID = 0;

	for (unsigned int y = 0; y < 16; y++)
	{
		//if (y > 14)
		//{

		//}
		//else if (y < 3)
		//{

		//}
		//else
		//{

		//}
		for (unsigned int x = 0; x < 16; x++)
		{
			for (unsigned int z = 0; z < 16; z++)
			{
				std::cout << x + y + z << "\n";
				chunk[x + y + z].BlockID = localBID;
				chunk[x + y + z].ZX = (z * 16) + x;
				chunk[x + y + z].Y = y;
			}
		}
	}

	return chunk;
}

static void printChunk(Block* chunk)
{
	for (unsigned int y = 0; y < 16; y++)
	{
		for (unsigned int x = 0; x < 16; x++)
		{
			for (unsigned int z = 0; z < 16; z++)
			{
				std::cout << "Block ID: " << static_cast<int>(chunk[x + y + z].BlockID)
					<< " X and Z: " << static_cast<int>(chunk[x + y + z].ZX)
					<< " Y: " << chunk[x + y + z].Y << "\n";
			}
		}
	}

	//for (int i = 0; i < 4095; i++)
	//{
	//	std::cout << "Block ID: " << static_cast<int>(chunk[i].BlockID) << " X and Z: " << static_cast<int>(chunk[i].XandZ) << " Y: " << chunk[i].Y << "\n";
	//}
}