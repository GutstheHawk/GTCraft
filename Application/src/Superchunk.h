#pragma once
#define SCX 1
#define SCY 1
#define SCZ 1

typedef glm::tvec4<GLbyte> byte4;

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ChunkGeneration.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <fstream>
#include <string>

struct Superchunk
{
	Chunk* sChunk[SCX][SCY][SCZ];

	Superchunk()
	{
		memset(sChunk, 0, sizeof sChunk);
        fillSuperchunk();
        generateHightmap();
        applyHightmap();
	}

	~Superchunk()
	{
		/*for (int x = 0; x < SCX; x++)
			for (int y = 0; y < SCX; y++)
				for (int z = 0; z < SCX; z++)
					delete sChunk[x][y][z];*/
	}

    uint8_t get(int x, int y, int z) {
        int cx = x / CX;
        int cy = y / CY;
        int cz = z / CZ;

        x %= CX;
        y %= CY;
        z %= CZ;

        if (!sChunk[cx][cy][cz])
            return 0;
        else
            return sChunk[cx][cy][cz]->getBlock(x, y, z);
    }

    void set(int x, int y, int z, uint8_t type) {
        int cx = x / CX;
        int cy = y / CY;
        int cz = z / CZ;

        x %= CX;
        y %= CY;
        z %= CZ;

        if (!sChunk[cx][cy][cz])
            sChunk[cx][cy][cz] = new Chunk();

        sChunk[cx][cy][cz]->setBlock(x, y, z, type);
    }

    void render(Camera* cam, Shader* shader) {
        for (int x = 0; x < SCX; x++)
            for (int y = 0; y < SCY; y++)
                for (int z = 0; z < SCZ; z++)
                    if (sChunk[x][y][z])
                    {
                        glm::mat4 projection = cam->ReturnProjection();
                        glm::mat4 view = cam->ReturnView();
                        glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x * CX, y * CY, z * CZ));

                        glm::mat4 pvm = projection * view * model;
                        shader->SetUniformMatrix4fv("pvm", 1, GL_FALSE, &pvm[0][0]);

                        sChunk[x][y][z]->render();
                    }
    }

    void fillSuperchunk()
    {
        for (int x = 0; x < SCX; x++)
            for (int y = 0; y < SCY; y++)
                for (int z = 0; z < SCZ; z++)
                    if (!sChunk[x][y][z])
                    {
                        Chunk* chunk = new Chunk();
                        sChunk[x][y][z] = chunk;
                    }
    }

	void generateHightmap()
	{

		float yFactor = 1.0f / (CX - 1);
		float xFactor = 1.0f / (CZ - 1);

		//float a = glm::linearRand(0.0f, 2.0f);
		//float b = glm::linearRand(0.0f, 2.0f);

        float a = 1.0f;
        float b = 2.0f;

		int index = 0;

        for (int x = 0; x < SCX; x++)
            for (int y = 0; y < SCY; y++)
                for (int z = 0; z < SCZ; z++)
                    if (sChunk[x][y][z])
                    {
                        float heightmapPregen[CX][CZ];

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
                                sChunk[x][y][z]->heightmap[row][col] = static_cast<int>(heightmapPregen[row][col] * 10);
                                //std::cout << index << " " << heightmap[row][col] << std::endl;
                            }
                        }
                    }

	}
    
    void applyHightmap()
    {
        for (int x = 0; x < SCX; x++)
            for (int y = 0; y < SCY; y++)
                for (int z = 0; z < SCZ; z++)
                {
                    sChunk[x][y][z]->applyHeightmap();
                }
    }


};