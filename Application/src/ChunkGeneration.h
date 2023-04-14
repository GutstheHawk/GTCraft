#pragma once
#define CX 16
#define CY 16
#define CZ 16
#define SEED 1999.0f

typedef glm::tvec4<GLbyte> byte4;

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <glm/gtc/noise.hpp>

#include <iostream>
#include <fstream>
#include <string>

struct Block {
	unsigned char BlockID;
	unsigned char ZX;
	unsigned short Y;
};

struct Chunk 
{
	uint8_t blocks[CX][CY][CZ];
	int elements;
	bool changed;
	VertexBuffer* chunkBuffer;
	unsigned int vbo;

	int heightmap[CX][CZ];

	Chunk() 
	{
		memset(blocks, 0, sizeof(blocks));
		elements = 0;
		changed = true;
		glGenBuffers(1, &vbo);

		fillWithDirt();

		//generateHightmap();
	}

	~Chunk()
	{

	}

	uint8_t getBlock(int x, int y, int z)
	{
		return blocks[x][y][z];
	}

	void setBlock(int x, int y, int z, uint8_t type)
	{
		blocks[x][y][z] = type;
		changed = true;
	}

	void update()
	{
		changed = false;

		byte4 vertex[CX * CY * CZ * 6 * 6];
		int i = 0;

		for (int x = 0; x < CX; x++)
		{
			for (int y = 0; y < CY; y++)
			{
				for (int z = 0; z < CZ; z++)
				{
					uint8_t type = blocks[x][y][z];

					if (!type)
						continue;

					// View from negative x
					if ((x - 1) < 0)
					{
						vertex[i++] = byte4(x, y,     z,	 type);
						vertex[i++] = byte4(x, y,     z + 1, type);
						vertex[i++] = byte4(x, y + 1, z,     type);
						vertex[i++] = byte4(x, y + 1, z,	 type);
						vertex[i++] = byte4(x, y,     z + 1, type);
						vertex[i++] = byte4(x, y + 1, z + 1, type);
					}
					else
					{
						if (!blocks[x - 1][y][z])
						{
							vertex[i++] = byte4(x, y,	  z,	 type);
							vertex[i++] = byte4(x, y,	  z + 1, type);
							vertex[i++] = byte4(x, y + 1, z,	 type);
							vertex[i++] = byte4(x, y + 1, z,	 type);
							vertex[i++] = byte4(x, y,	  z + 1, type);
							vertex[i++] = byte4(x, y + 1, z + 1, type);
						}
					}

					// View from positive x
					if ((x + 1) == CX)
					{
						vertex[i++] = byte4(x + 1, y,     z,     type);
						vertex[i++] = byte4(x + 1, y + 1, z,     type);
						vertex[i++] = byte4(x + 1, y,     z + 1, type);
						vertex[i++] = byte4(x + 1, y + 1, z,	 type);
						vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
						vertex[i++] = byte4(x + 1, y,     z + 1, type);
					}
					else
					{
						if (!blocks[x + 1][y][z])
						{
							vertex[i++] = byte4(x + 1, y,     z,     type);
							vertex[i++] = byte4(x + 1, y + 1, z,     type);
							vertex[i++] = byte4(x + 1, y,     z + 1, type);
							vertex[i++] = byte4(x + 1, y + 1, z,     type);
							vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
							vertex[i++] = byte4(x + 1, y,     z + 1, type);
						}
					}

					// View from negative y
					if ((y - 1) < 0)
					{
						vertex[i++] = byte4(x,     y, z,     type);
						vertex[i++] = byte4(x + 1, y, z,     type);
						vertex[i++] = byte4(x,     y, z + 1, type);
						vertex[i++] = byte4(x + 1, y, z,     type);
						vertex[i++] = byte4(x + 1, y, z + 1, type);
						vertex[i++] = byte4(x,     y, z + 1, type);
					}
					else
					{
						if (!blocks[x][y - 1][z])
						{
							vertex[i++] = byte4(x,	   y, z,	 type);
							vertex[i++] = byte4(x + 1, y, z,	 type);
							vertex[i++] = byte4(x,	   y, z + 1, type);
							vertex[i++] = byte4(x + 1, y, z,	 type);
							vertex[i++] = byte4(x + 1, y, z + 1, type);
							vertex[i++] = byte4(x,     y, z + 1, type);
						}
					}

					// View from positive y
					if ((y + 1) == CY)
					{
						vertex[i++] = byte4(x,     y + 1, z,	 type);
						vertex[i++] = byte4(x,	   y + 1, z + 1, type);
						vertex[i++] = byte4(x + 1, y + 1, z,	 type);
						vertex[i++] = byte4(x,     y + 1, z + 1, type);
						vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
						vertex[i++] = byte4(x + 1, y + 1, z,	 type);
					}
					else
					{
						if (!blocks[x][y + 1][z])
						{
							vertex[i++] = byte4(x,     y + 1, z,     type);
							vertex[i++] = byte4(x,     y + 1, z + 1, type);
							vertex[i++] = byte4(x + 1, y + 1, z,     type);
							vertex[i++] = byte4(x,     y + 1, z + 1, type);
							vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
							vertex[i++] = byte4(x + 1, y + 1, z,     type);
						}
					}

					// View from negative z
					if ((z - 1) < 0)
					{
						vertex[i++] = byte4(x,     y,	  z, type);
						vertex[i++] = byte4(x,     y + 1, z, type);
						vertex[i++] = byte4(x + 1, y,	  z, type);
						vertex[i++] = byte4(x,     y + 1, z, type);
						vertex[i++] = byte4(x + 1, y + 1, z, type);
						vertex[i++] = byte4(x + 1, y,	  z, type);
					}
					else
					{
						if (!blocks[x][y][z - 1])
						{
							vertex[i++] = byte4(x,     y,     z, type);
							vertex[i++] = byte4(x,     y + 1, z, type);
							vertex[i++] = byte4(x + 1, y,     z, type);
							vertex[i++] = byte4(x,     y + 1, z, type);
							vertex[i++] = byte4(x + 1, y + 1, z, type);
							vertex[i++] = byte4(x + 1, y,     z, type);
						}
					}


					// View from positive z
					if ((z + 1) == CZ)
					{
						vertex[i++] = byte4(x,     y,     z + 1, type);
						vertex[i++] = byte4(x + 1, y,     z + 1, type);
						vertex[i++] = byte4(x,     y + 1, z + 1, type);
						vertex[i++] = byte4(x,     y + 1, z + 1, type);
						vertex[i++] = byte4(x + 1, y,     z + 1, type);
						vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
					}
					else
					{
						if (!blocks[x][y][z + 1])
						{
							vertex[i++] = byte4(x,     y,     z + 1, type);
							vertex[i++] = byte4(x + 1, y,     z + 1, type);
							vertex[i++] = byte4(x,     y + 1, z + 1, type);
							vertex[i++] = byte4(x,     y + 1, z + 1, type);
							vertex[i++] = byte4(x + 1, y,     z + 1, type);
							vertex[i++] = byte4(x + 1, y + 1, z + 1, type);
						}
					}
				}
			}
		}

		elements = i;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, elements * sizeof * vertex, vertex, GL_STATIC_DRAW);
		//VertexBuffer vbo(vertex, elements * sizeof(vertex), GL_STATIC_DRAW);
		//chunkBuffer = &vbo;
	}

	void render()
	{
		if (changed)
			update();

		if (!elements)
			return;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, elements);
	}

	void fillWithDirt()
	{
		for (int x = 0; x < CX; x++)
		{
			for (int y = 0; y < CY; y++)
			{
				for (int z = 0; z < CZ; z++)
				{
					setBlock(x, y, z, 1);
				}
			}
		}
	}

	void generateHightmap()
	{
		float heightmapPregen[CX][CZ];

		float yFactor = 1.0f / (CX - 1);
		float xFactor = 1.0f / (CZ - 1);
		float a = 1.0f;
		float b = 2.0f;

		int index = 0;

		for (int row = 0; row < CX; row++)
		{
			for (int col = 0; col < CZ; col++)
			{
				float x = xFactor * col;
				float y = yFactor * row;
				float sum = 0.0f;
				float freq = a;
				float scale = b;

				// Compute the sum for each octave
				for (int oct = 0; oct < 4; oct++)
				{
					glm::vec2 p(x * freq, y * freq);
					float val = glm::perlin(p) / scale;
					sum += val;
					float result = (sum + 1.0f) / 2.0f;

					// Store in texture buffer
					heightmapPregen[row][col] = result;
					freq *= 2.0f;   // Double the frequency
					scale *= b;
				}
			}
		}


		for (int row = 0; row < CX; row++)
		{
			for (int col = 0; col < CZ; col++)
			{
				index++;
				heightmap[row][col] = static_cast<int>(heightmapPregen[row][col] * 10);
				//std::cout << index << " " << heightmap[row][col] << std::endl;
			}
		}

	}

	void applyHeightmap()
	{
		int bottomStartingHeight = (CY - 1) - 10;


		for (int y = bottomStartingHeight; y < CY; y++)
		{
			for (int x = 0; x < CX; x++)
			{
				for (int z = 0; z < CZ; z++)
				{
					//std::cout << heightmap[x][z] << std::endl;
					setBlock(x, y, z, 0);
					if (heightmap[x][z] >= (y - bottomStartingHeight))
					{
						setBlock(x, y, z, 1);
					}
				}
			}
		}
	}
};

