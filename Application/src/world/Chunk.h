#pragma once
#define CX 16
#define CY 16
#define CZ 16
#define SEED 1999.0f

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "BlockDefinitions.h"
#include "VertexBuildingBlocks.h"

#include <glm/gtc/noise.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

struct Chunk
{
	std::unique_ptr<std::unique_ptr<std::unique_ptr<uint8_t[]>[]>[]> blocks;
	std::unique_ptr<Vertex[]> vertexes;
	int elements;
	bool changed;
	int worldPosX;
	int worldPosZ;

	std::shared_ptr<std::set<uint8_t>> transparentBlocks;
	std::shared_ptr<std::unordered_map<uint8_t, uint8_t>> twoSidedBlocks;
	std::shared_ptr<std::unordered_map<uint8_t, std::pair<uint8_t, uint8_t>>> threeSidedBlocks;

	int maxChunkVertices = (CX * CY * CZ * 6 * 6);
	unsigned int vbo;
	unsigned int vao;

	Chunk() :
		blocks(std::make_unique<std::unique_ptr<std::unique_ptr<uint8_t[]>[]>[]>(CX)),
		vertexes(std::make_unique<Vertex[]>(CX* CY* CZ * 6 * 6)),
		elements(0),
		changed(true),
		worldPosX(0),
		worldPosZ(0),
		twoSidedBlocks(std::make_shared<std::unordered_map<uint8_t, uint8_t>>()),
		threeSidedBlocks(std::make_shared<std::unordered_map<uint8_t, std::pair<uint8_t, uint8_t>>>()),
		vbo(0),
		vao(0)
	{
		for (int i = 0; i < CX; i++) {
			blocks[i] = std::make_unique<std::unique_ptr<uint8_t[]>[]>(CY);
			for (int j = 0; j < CY; j++) {
				blocks[i][j] = std::make_unique<uint8_t[]>(CZ);
			}
		}

		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);

		//generateHightmap();
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(blocks, vertexes, elements, changed, worldPosX, worldPosZ,
			twoSidedBlocks, threeSidedBlocks, maxChunkVertices, vbo, vao);
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

		int i = 0;

		uint8_t texIndex = 0;
		uint8_t lightingVal = 0;


		for (uint8_t x = 0; x < CX; x++)
		{
			for (uint8_t y = 0; y < CY; y++)
			{
				for (uint8_t z = 0; z < CZ; z++)
				{
					uint8_t type = blocks[x][y][z];

					if (!type)
						continue;

					if (twoSidedBlocks->count(type))
					{
						// View from negative x
						if (((x - 1) < 0) || (!blocks[x - 1][y][z]))
						{
							genNegXSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if (transparentBlocks->find(blocks[x - 1][y][z]) != transparentBlocks->end())
								genNegXSide(vertexes.get(), &x, &y, &z, type, &i);
						}

						// View from positive x
						if (((x + 1) == CX) || (!blocks[x + 1][y][z]))
						{
							genPosXSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if (transparentBlocks->find(blocks[x + 1][y][z]) != transparentBlocks->end())
								genPosXSide(vertexes.get(), &x, &y, &z, type, &i);

						}

						// View from negative y
						if (((y - 1) < 0) || (!blocks[x][y - 1][z]))
						{
							genNegYSide(vertexes.get(), &x, &y, &z, twoSidedBlocks->at(type), &i);
						}
						else
						{
							if (transparentBlocks->find(blocks[x][y - 1][z]) != transparentBlocks->end())
								genNegYSide(vertexes.get(), &x, &y, &z, twoSidedBlocks->at(type), &i);

						}

						// View from positive y
						if (((y + 1) == CY) || (!blocks[x][y + 1][z]))
						{
							genPosYSide(vertexes.get(), &x, &y, &z, twoSidedBlocks->at(type), &i);
						}
						else
						{
							if (transparentBlocks->find(blocks[x][y + 1][z]) != transparentBlocks->end())
								genPosYSide(vertexes.get(), &x, &y, &z, twoSidedBlocks->at(type), &i);

						}

						// View from negative z
						if (((z - 1) < 0) || (!blocks[x][y][z - 1]))
						{
							genNegZSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if (transparentBlocks->find(blocks[x][y][z - 1]) != transparentBlocks->end())
								genNegZSide(vertexes.get(), &x, &y, &z, type, &i);

						}

						// View from positive z
						if (((z + 1) == CZ) || (!blocks[x][y][z + 1]))
						{
							genPosZSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if (transparentBlocks->find(blocks[x][y][z + 1]) != transparentBlocks->end())
								genPosZSide(vertexes.get(), &x, &y, &z, type, &i);

						}
					}
					else if (threeSidedBlocks->count(type))
					{
						// View from negative x
						if (((x - 1) < 0) || (!blocks[x - 1][y][z]))
						{
							genNegXSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);
						}
						else
						{
							if (blocks[x - 1][y][z] == WATER)
								genNegXSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);
						}


						// View from positive x
						if (((x + 1) == CX) || (!blocks[x + 1][y][z]))
						{
							genPosXSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);
						}
						else
						{
							if (blocks[x + 1][y][z] == WATER)
								genPosXSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);

						}

						// View from negative y
						if (((y - 1) < 0) || (!blocks[x][y - 1][z]))
						{
							genNegYSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).second, &i);
						}
						else
						{
							if (blocks[x][y - 1][z] == WATER)
								genNegYSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).second, &i);

						}

						// View from positive y
						if (((y + 1) == CY) || (!blocks[x][y + 1][z]))
						{
							genPosYSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if (blocks[x][y + 1][z] == WATER)
								genPosYSide(vertexes.get(), &x, &y, &z, type, &i);

						}

						// View from negative z
						if (((z - 1) < 0) || (!blocks[x][y][z - 1]))
						{
							genNegZSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);
						}
						else
						{
							if (blocks[x][y][z - 1] == WATER)
								genNegZSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);

						}

						// View from positive z
						if (((z + 1) == CZ) || (!blocks[x][y][z + 1]))
						{
							genPosZSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);
						}
						else
						{
							if (blocks[x][y][z + 1] == WATER)
								genPosZSide(vertexes.get(), &x, &y, &z, threeSidedBlocks->at(type).first, &i);

						}
					}
					else
					{
						// View from negative x
						if ((((x - 1) < 0) || (!blocks[x - 1][y][z])) && !(type == WATER))
						{
							genNegXSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if ((blocks[x - 1][y][z] == WATER) && !(type == WATER))
								genNegXSide(vertexes.get(), &x, &y, &z, type, &i);
						}


						// View from positive x
						if ((((x + 1) == CX) || (!blocks[x + 1][y][z])) && !(type == WATER))
						{
							genPosXSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if ((blocks[x + 1][y][z] == WATER) && !(type == WATER))
								genPosXSide(vertexes.get(), &x, &y, &z, type, &i);
						}

						// View from negative y
						if ((((y - 1) < 0) || (!blocks[x][y - 1][z])) && !(type == WATER))
						{
							genNegYSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if ((blocks[x][y - 1][z] == WATER) && !(type == WATER))
								genNegYSide(vertexes.get(), &x, &y, &z, type, &i);

						}

						// View from positive y
						if ((((y + 1) == CY) || (!blocks[x][y + 1][z])) && !(type == WATER))
						{
							genPosYSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if ((blocks[x][y + 1][z] == WATER) && !(type == WATER))
								genPosYSide(vertexes.get(), &x, &y, &z, type, &i);

						}

						// View from negative z
						if ((((z - 1) < 0) || (!blocks[x][y][z - 1])) && !(type == WATER))
						{
							genNegZSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if ((blocks[x][y][z - 1] == WATER) && !(type == WATER))
								genNegZSide(vertexes.get(), &x, &y, &z, type, &i);

						}

						// View from positive z
						if ((((z + 1) == CZ) || (!blocks[x][y][z + 1])) && !(type == WATER))
						{
							genPosZSide(vertexes.get(), &x, &y, &z, type, &i);
						}
						else
						{
							if ((blocks[x][y][z + 1] == WATER) && !(type == WATER))
								genPosZSide(vertexes.get(), &x, &y, &z, type, &i);

						}
					}

				}
			}
		}

		elements = i;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, maxChunkVertices * sizeof(Vertex), vertexes.get(), GL_STATIC_DRAW);
	}

	void render()
	{
		if (changed)
			update();

		if (!elements)
			return;
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texIndex_lightingVal_type));
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, elements);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void fillWithBlock(uint8_t blockType)
	{
		for (int x = 0; x < CX; x++)
		{
			for (int y = 0; y < CY; y++)
			{
				for (int z = 0; z < CZ; z++)
				{
					setBlock(x, y, z, blockType);
				}
			}
		}
	}

	void fillWithAir()
	{
		for (int x = 0; x < CX; x++)
		{
			for (int y = 0; y < CY; y++)
			{
				for (int z = 0; z < CZ; z++)
				{
					setBlock(x, y, z, 0);
				}
			}
		}
	}

};

