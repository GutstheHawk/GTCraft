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

#include <glm/gtc/noise.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>

typedef glm::tvec3<GLubyte> ubyte3;

#pragma pack(push, 1)
struct Vertex {
	ubyte3 Position;
	ubyte3 texIndex_lightingVal_type;
};
#pragma pack(pop)

struct Chunk 
{
	uint8_t*** blocks;
	Vertex* vertexes;
	int elements;
	bool changed;
	int worldPosX;
	int worldPosZ;

	int maxChunkVertices = (CX * CY * CZ * 6 * 6);
	//VertexBuffer* chunkVertexBuffer;
	//VertexBufferLayout* chunkBufferLayout;
	//VertexArray* chunkVertexArray;
	//IndexBuffer* chunkIndexBuffer;
	unsigned int vbo;
	unsigned int vao;

	int heightmap[CX][CZ];

	Chunk()
	{
		blocks =  new uint8_t ** [CX];
		for (int i = 0; i < CX; i++) {
			blocks[i] = new uint8_t * [CY];
			for (int j = 0; j < CY; j++) {
				blocks[i][j] = new uint8_t[CZ];
			}
		}

		//memset(blocks[0][0], 0, CX * CY * CZ * sizeof(uint8_t));

		vertexes = new Vertex[CX * CY * CZ * 6 * 6];
		//memset(blocks, 0, sizeof(blocks));
		elements = 0;
		changed = true;
		worldPosX = 0;
		worldPosZ = 0;
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);
		//genIndexBuffer();

		//fillWithDirt();

		//generateHightmap();
	}

	~Chunk()
	{
		for (int i = 0; i < CX; i++) {
			for (int j = 0; j < CY; j++) {
				delete[] blocks[i][j];
			}
			delete[] blocks[i];
		}
		delete[] blocks;

		delete[] vertexes;
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

					// View from negative x
					if (((x - 1) < 0) || (!blocks[x - 1][y][z]))
					{
						Vertex v0 = {};
						v0.Position = ubyte3(x, y, z);
						texIndex = { 0 };
						lightingVal = { 153 };
						v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v0;

						Vertex v1 = {};
						v1.Position = ubyte3(x, y, z + 1);
						texIndex = { 3 };
						lightingVal = { 153 };
						v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v1;

						Vertex v2 = {};
						v2.Position = ubyte3(x, y + 1, z);
						texIndex = { 1 };
						lightingVal = { 153 };
						v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v2;

						Vertex v3 = {};
						v3.Position = ubyte3(x, y + 1, z);
						texIndex = { 1 };
						lightingVal = { 153 };
						v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v3;

						Vertex v4 = {};
						v4.Position = ubyte3(x, y, z + 1);
						texIndex = { 3 };
						lightingVal = { 153 };
						v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v4;

						Vertex v5 = {};
						v5.Position = ubyte3(x, y + 1, z + 1);
						texIndex = { 2 };
						lightingVal = { 153 };
						v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v5;
					}


					// View from positive x
					if (((x + 1) == CX) || (!blocks[x + 1][y][z]))
					{
						Vertex v0 = {};
						v0.Position = ubyte3( x + 1, y, z );
						texIndex = { 0 };
						lightingVal = { 153 };
						v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v0;

						Vertex v1 = {};
						v1.Position = ubyte3( x + 1, y + 1, z );
						texIndex = { 1 };
						lightingVal = { 153 };
						v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v1;

						Vertex v2 = {};
						v2.Position = ubyte3( x + 1, y, z + 1 );
						texIndex = { 3 };
						lightingVal = { 153 };
						v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v2;

						Vertex v3 = {};
						v3.Position = ubyte3( x + 1, y + 1, z );
						texIndex = { 1 };
						lightingVal = { 153 };
						v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v3;

						Vertex v4 = {};
						v4.Position = ubyte3( x + 1, y + 1, z + 1 );
						texIndex = { 2 };
						lightingVal = { 153 };
						v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v4;

						Vertex v5 = {};
						v5.Position = ubyte3( x + 1, y, z + 1 );
						texIndex = { 3 };
						lightingVal = { 153 };
						v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v5;
					}

					// View from negative y
					if (((y - 1) < 0) || (!blocks[x][y - 1][z]))
					{
						Vertex v0 = {};
						v0.Position = ubyte3( x, y, z );
						texIndex = { 0 };
						lightingVal = { 102 };
						v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v0;

						Vertex v1 = {};
						v1.Position = ubyte3( x + 1, y, z );
						texIndex = { 3 };
						lightingVal = { 102 };
						v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v1;

						Vertex v2 = {};
						v2.Position = ubyte3( x, y, z + 1 );
						texIndex = { 1 };
						lightingVal = { 102 };
						v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v2;

						Vertex v3 = {};
						v3.Position = ubyte3( x + 1, y, z );
						texIndex = { 3 };
						lightingVal = { 102 };
						v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v3;

						Vertex v4 = {};
						v4.Position = ubyte3( x + 1, y, z + 1 );
						texIndex = { 2 };
						lightingVal = { 102 };
						v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v4;

						Vertex v5 = {};
						v5.Position = ubyte3( x, y, z + 1 );
						texIndex = { 1 };
						lightingVal = { 102 };
						v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v5;
					}

					// View from positive y
					if (((y + 1) == CY) || (!blocks[x][y + 1][z]))
					{
						Vertex v0 = {};
						v0.Position = ubyte3( x, y + 1, z );
						texIndex = { 0 };
						lightingVal = { 254 };
						v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v0;

						Vertex v1 = {};
						v1.Position = ubyte3( x, y + 1, z + 1 );
						texIndex = { 1 };
						lightingVal = { 254 };
						v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v1;

						Vertex v2 = {};
						v2.Position = ubyte3( x + 1, y + 1, z );
						texIndex = { 3 };
						lightingVal = { 254 };
						v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v2;

						Vertex v3 = {};
						v3.Position = ubyte3( x, y + 1, z + 1 );
						texIndex = { 1 };
						lightingVal = { 254 };
						v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v3;

						Vertex v4 = {};
						v4.Position = ubyte3( x + 1, y + 1, z + 1 );
						texIndex = { 2 };
						lightingVal = { 254 };
						v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v4;

						Vertex v5 = {};
						v5.Position = ubyte3( x + 1, y + 1, z );
						texIndex = { 3 };
						lightingVal = { 254 };
						v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v5;
					}

					// View from negative z
					if (((z - 1) < 0) || (!blocks[x][y][z - 1]))
					{
						Vertex v0 = {};
						v0.Position = ubyte3( x, y, z );
						texIndex = { 0 };
						lightingVal = { 203 };
						v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v0;

						Vertex v1 = {};
						v1.Position = ubyte3( x, y + 1, z );
						texIndex = { 3 };
						lightingVal = { 203 };
						v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v1;

						Vertex v2 = {};
						v2.Position = ubyte3( x + 1, y, z );
						texIndex = { 1 };
						lightingVal = { 203 };
						v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v2;

						Vertex v3 = {};
						v3.Position = ubyte3( x, y + 1, z );
						texIndex = { 3 };
						lightingVal = { 203 };
						v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v3;

						Vertex v4 = {};
						v4.Position = ubyte3( x + 1, y + 1, z );
						texIndex = { 2 };
						lightingVal = { 203 };
						v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v4;

						Vertex v5 = {};
						v5.Position = ubyte3( x + 1, y, z );
						texIndex = { 1 };
						lightingVal = { 203 };
						v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v5;
					}


					// View from positive z
					if (((z + 1) == CZ) || (!blocks[x][y][z + 1]))
					{
						Vertex v0 = {};
						v0.Position = ubyte3( x, y, z + 1 );
						texIndex = { 0 };
						lightingVal = { 203 };
						v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v0;

						Vertex v1 = {};
						v1.Position = ubyte3( x + 1, y, z + 1 );
						texIndex = { 1 };
						lightingVal = { 203 };
						v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v1;

						Vertex v2 = {};
						v2.Position = ubyte3( x, y + 1, z + 1 );
						texIndex = { 3 };
						lightingVal = { 203 };
						v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v2;

						Vertex v3 = {};
						v3.Position = ubyte3( x, y + 1, z + 1 );
						texIndex = { 3 };
						lightingVal = { 203 };
						v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v3;

						Vertex v4 = {};
						v4.Position = ubyte3( x + 1, y, z + 1 );
						texIndex = { 1 };
						lightingVal = { 203 };
						v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v4;

						Vertex v5 = {};
						v5.Position = ubyte3( x + 1, y + 1, z + 1 );
						texIndex = { 2 };
						lightingVal = { 203 };
						v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
						vertexes[i++] = v5;
					}
				}
			}
		}

		elements = i;

		/*VertexBuffer vb(vertexes, elements * sizeof* vertexes, GL_STATIC_DRAW);
		chunkVertexBuffer = &vb;

		VertexBufferLayout vbl;
		vbl.Push<unsigned int>(3, GL_FALSE);
		vbl.Push<float>(2, GL_FALSE);
		vbl.Push<int8_t>(1, GL_FALSE);
		chunkBufferLayout = &vbl;*/
		//std::cout << blocks[2][2][2] << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, maxChunkVertices * sizeof(Vertex), vertexes, GL_STATIC_DRAW);

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

		/*IndexBuffer ib(indices, maxIndexCount);
		chunkIndexBuffer = &ib;*/

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

		//VertexArray chunkVA;
		//chunkVA.AddBuffer(*chunkVertexBuffer, *chunkBufferLayout);
		//chunkVA.Bind();
		//chunkVertexArray = &chunkVA;

		//chunkIndexBuffer->Bind();

		//glDrawElements(GL_TRIANGLES, size / sizeof(GLfloat), GL_UNSIGNED_INT, GL_STATIC_DRAW);
		////glDrawArrays(GL_TRIANGLES, 0, elements);

		//chunkIndexBuffer->Unbind();
		//chunkVA.Unbind();
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

	void generateHightmap()
	{
		float heightmapPregen[CX][CZ];

		float yFactor = 1.0f / (CX);
		float xFactor = 1.0f / (CZ);
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
		//int bottomStartingHeight = (CY - 1) - 10;


		for (int y = 0; y < CY; y++)
		{
			for (int x = 0; x < CX; x++)
			{
				for (int z = 0; z < CZ; z++)
				{
					//std::cout << heightmap[x][z] << std::endl;
					if (y < heightmap[x][z])
					{
						setBlock(x, y, z, DIRT);
					}
					if (y == heightmap[x][z])
					{
						setBlock(x, y, z, GRASS);
					}
				}
			}
		}
	}

	void getHeightmap(int8_t** sChunkHeightmap)
	{
		for (int x = 0; x < CX; x++)
			for (int z = 0; z < CZ; z++)
			{
				heightmap[x][z] = sChunkHeightmap[(worldPosX * 16) + x][(worldPosZ * 16) + z];
			}
	}

	/*void genPosXSide(int* x, int* y, int* z, uint8_t* type, int* i)
	{
		Vertex v0 = {};
		v0.Position = byte3(*x, *y, *z);
		v0.texIndex = { 0 };
		v0.lightingVal = { 0 };
		v0.type = *type;
		vertexes[*i++] = v0;

		Vertex v1 = {};
		v1.Position = byte3(*x, *y, *z + 1);
		v1.texIndex = { 1 };
		v1.lightingVal = { 0 };
		v1.type = *type;
		vertexes[*i++] = v1;

		Vertex v2 = {};
		v2.Position = byte3(*x, *y + 1, *z);
		v2.texIndex = { 3 };
		v2.lightingVal = { 0 };
		v2.type = *type;
		vertexes[*i++] = v2;

		Vertex v3 = {};
		v3.Position = byte3(*x, *y + 1, *z);
		v3.texIndex = { 3 };
		v3.lightingVal = { 0 };
		v3.type = *type;
		vertexes[*i++] = v3;

		Vertex v4 = {};
		v4.Position = byte3(*x, *y, *z + 1);
		v4.texIndex = { 1 };
		v4.lightingVal = { 0 };
		v4.type = *type;
		vertexes[*i++] = v4;

		Vertex v5 = {};
		v5.Position = byte3(*x, *y + 1, *z + 1);
		v5.texIndex = { 2 };
		v5.lightingVal = { 0 };
		v5.type = *type;
		vertexes[*i++] = v5;
	}*/
};

