#pragma once
#define CX 16
#define CY 16
#define CZ 16
#define SEED 1999.0f

typedef glm::tvec4<GLbyte> byte4;

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include <glm/gtc/noise.hpp>

#include <iostream>
#include <fstream>
#include <string>

struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
	uint8_t type;
};

struct Chunk 
{

	uint8_t blocks[CX][CY][CZ];
	int elements;
	bool changed;
	VertexBuffer* chunkVertexBuffer;
	VertexBufferLayout* chunkBufferLayout;
	VertexArray* chunkVertexArray;
	IndexBuffer* chunkIndexBuffer;
	//unsigned int vbo;

	int heightmap[CX][CZ];

	Chunk()
	{
		memset(blocks, 0, sizeof(blocks));
		elements = 0;
		changed = true;
		//glGenBuffers(1, &vbo);
		genIndexBuffer();

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

		Vertex vertexes[CX * CY * CZ * 4 * 6];
		int i = 0;

		for (unsigned int x = 0; x < CX; x++)
		{
			for (unsigned int y = 0; y < CY; y++)
			{
				for (unsigned int z = 0; z < CZ; z++)
				{
					uint8_t type = blocks[x][y][z];

					if (!type)
						continue;

					// View from negative x
					if ((x - 1) < 0)
					{
						Vertex v0;
						v0.Position = { x, y, z };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x, y, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x, y + 1, z };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;

						/*vertexes[i++] = byte4(x, y,     z,	 type);
						vertexes[i++] = byte4(x, y,     z + 1, type);
						vertexes[i++] = byte4(x, y + 1, z,     type);
						vertexes[i++] = byte4(x, y + 1, z,	 type);
						vertexes[i++] = byte4(x, y,     z + 1, type);
						vertexes[i++] = byte4(x, y + 1, z + 1, type);*/
					}
					else
					{
						Vertex v0;
						v0.Position = { x, y, z };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x, y, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x, y + 1, z };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
					}

					// View from positive x
					if ((x + 1) == CX)
					{
						Vertex v0;
						v0.Position = { x + 1, y, z };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x + 1, y, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x + 1, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x + 1, y + 1, z };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
					}
					else
					{
						if (!blocks[x + 1][y][z])
						{
							Vertex v0;
							v0.Position = { x + 1, y, z };
							v0.TexCoords = { 0.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v0;

							Vertex v1;
							v0.Position = { x + 1, y, z + 1 };
							v0.TexCoords = { 1.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v1;

							Vertex v2;
							v0.Position = { x + 1, y + 1, z + 1 };
							v0.TexCoords = { 1.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v2;

							Vertex v3;
							v0.Position = { x + 1, y + 1, z };
							v0.TexCoords = { 0.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v3;
						}
					}

					// View from negative y
					if ((y - 1) < 0)
					{
						Vertex v0;
						v0.Position = { x, y, z };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x, y, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x + 1, y, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x + 1, y, z };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
						/*vertexes[i++] = byte4(x,     y, z,     type);
						vertexes[i++] = byte4(x + 1, y, z,     type);
						vertexes[i++] = byte4(x,     y, z + 1, type);
						vertexes[i++] = byte4(x + 1, y, z,     type);
						vertexes[i++] = byte4(x + 1, y, z + 1, type);
						vertexes[i++] = byte4(x,     y, z + 1, type);*/
					}
					else
					{
						if (!blocks[x][y - 1][z])
						{
							Vertex v0;
							v0.Position = { x, y, z };
							v0.TexCoords = { 0.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v0;

							Vertex v1;
							v0.Position = { x, y, z + 1 };
							v0.TexCoords = { 1.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v1;

							Vertex v2;
							v0.Position = { x + 1, y, z + 1 };
							v0.TexCoords = { 1.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v2;

							Vertex v3;
							v0.Position = { x + 1, y, z };
							v0.TexCoords = { 0.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v3;
						}
					}

					// View from positive y
					if ((y + 1) == CY)
					{
						Vertex v0;
						v0.Position = { x, y + 1, z };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x + 1, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x + 1, y + 1, z };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
					}
					else
					{
						if (!blocks[x][y + 1][z])
						{
							Vertex v0;
							v0.Position = { x, y + 1, z };
							v0.TexCoords = { 0.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v0;

							Vertex v1;
							v0.Position = { x, y + 1, z + 1 };
							v0.TexCoords = { 1.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v1;

							Vertex v2;
							v0.Position = { x + 1, y + 1, z + 1 };
							v0.TexCoords = { 1.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v2;

							Vertex v3;
							v0.Position = { x + 1, y + 1, z };
							v0.TexCoords = { 0.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v3;
						}
					}

					// View from negative z
					if ((z - 1) < 0)
					{
						Vertex v0;
						v0.Position = { x, y, z };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x + 1, y, z };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x + 1, y + 1, z };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x, y + 1, z };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
						/*vertexes[i++] = byte4(x,     y,	  z, type);
						vertexes[i++] = byte4(x,     y + 1, z, type);
						vertexes[i++] = byte4(x + 1, y,	  z, type);
						vertexes[i++] = byte4(x,     y + 1, z, type);
						vertexes[i++] = byte4(x + 1, y + 1, z, type);
						vertexes[i++] = byte4(x + 1, y,	  z, type);*/
					}
					else
					{
						if (!blocks[x][y][z - 1])
						{
							Vertex v0;
							v0.Position = { x, y, z };
							v0.TexCoords = { 0.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v0;

							Vertex v1;
							v0.Position = { x + 1, y, z };
							v0.TexCoords = { 1.0, 0.0 };
							v0.type = type;
							vertexes[i++] = v1;

							Vertex v2;
							v0.Position = { x + 1, y + 1, z };
							v0.TexCoords = { 1.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v2;

							Vertex v3;
							v0.Position = { x, y + 1, z };
							v0.TexCoords = { 0.0, 1.0 };
							v0.type = type;
							vertexes[i++] = v3;
						}
					}


					// View from positive z
					if ((z + 1) == CZ)
					{
						Vertex v0;
						v0.Position = { x, y, z + 1 };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x + 1, y, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x + 1, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x, y + 1, z + 1 };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
					}
					else
					{
						Vertex v0;
						v0.Position = { x, y, z + 1 };
						v0.TexCoords = { 0.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v0;

						Vertex v1;
						v0.Position = { x + 1, y, z + 1 };
						v0.TexCoords = { 1.0, 0.0 };
						v0.type = type;
						vertexes[i++] = v1;

						Vertex v2;
						v0.Position = { x + 1, y + 1, z + 1 };
						v0.TexCoords = { 1.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v2;

						Vertex v3;
						v0.Position = { x, y + 1, z + 1 };
						v0.TexCoords = { 0.0, 1.0 };
						v0.type = type;
						vertexes[i++] = v3;
					}
				}
			}
		}

		elements = i;

		VertexBuffer vb(vertexes, elements * sizeof* vertexes, GL_STATIC_DRAW);
		chunkVertexBuffer = &vb;

		VertexBufferLayout vbl;
		vbl.Push<unsigned int>(3, GL_FALSE);
		vbl.Push<float>(2, GL_FALSE);
		vbl.Push<int8_t>(1, GL_FALSE);
		chunkBufferLayout = &vbl;

		/*glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, elements * sizeof *vertexes, vertexes, GL_STATIC_DRAW);*/

		//VertexBuffer vbo(vertex, elements * sizeof(vertex), GL_STATIC_DRAW);
		//chunkBuffer = &vbo;
	}

	void genIndexBuffer()
	{
		const unsigned int maxQuadCount = (CX * CY * CZ * 6) / 2;
		const unsigned int maxVertexCount = maxQuadCount * 4;
		const unsigned int maxIndexCount = maxQuadCount * 6;

		unsigned int indices[maxIndexCount];
		unsigned int offset = 0;
		for (unsigned int i = 0; i < maxIndexCount; i+= 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		IndexBuffer ib(indices, maxIndexCount);
		chunkIndexBuffer = &ib;

	}

	void render()
	{
		if (changed)
			update();

		if (!elements)
			return;

		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 0, 0);

		VertexArray chunkVA;
		chunkVA.AddBuffer(*chunkVertexBuffer, *chunkBufferLayout);
		chunkVA.Bind();
		chunkVertexArray = &chunkVA;

		chunkIndexBuffer->Bind();

		glDrawElements(GL_TRIANGLES, size / sizeof(GLfloat), GL_UNSIGNED_INT, GL_STATIC_DRA);
		//glDrawArrays(GL_TRIANGLES, 0, elements);

		chunkIndexBuffer->Unbind();
		chunkVA.Unbind();
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

