#pragma once
#define SCX 1
#define SCY 2
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
#include <glm/glm.hpp>
#include <cmath>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

struct Superchunk
{
	Chunk* sChunk[SCX][SCY][SCZ];
    int8_t** heightmap;

	Superchunk()
	{
		memset(sChunk, 0, sizeof sChunk);

        heightmap = new int8_t * [SCX * 16];
        for (int i = 0; i < (SCX * 16); i++) {
            heightmap[i] = new int8_t[SCZ * 16];
        }

        fillSuperchunk();
        //generateSuperchunkHeightmap(42.0f);
        //setChunkHeightMaps();
        //applyHeightmaps();
        //generateHightmap();
        //applyHightmap();
	}

	~Superchunk()
	{
        for (int i = 0; i < (SCX * 16); i++) {
            delete[] heightmap[i];
        }
        delete[] heightmap;
		/*for (int x = 0; x < SCX; x++)
			for (int y = 0; y < SCX; y++)
				for (int z = 0; z < SCX; z++)
					delete sChunk[x][y][z];*/
	}

    bool intersectRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& tMin, float& tMax)
    {
        glm::vec3 invRayDirection = 1.0f / rayDirection;
        glm::vec3 tMinTemp = (aabbMin - rayOrigin) * invRayDirection;
        glm::vec3 tMaxTemp = (aabbMax - rayOrigin) * invRayDirection;

        tMin = glm::max(glm::max(glm::min(tMinTemp.x, tMaxTemp.x), glm::min(tMinTemp.y, tMaxTemp.y)), glm::min(tMinTemp.z, tMaxTemp.z));
        tMax = glm::min(glm::min(glm::max(tMinTemp.x, tMaxTemp.x), glm::max(tMinTemp.y, tMaxTemp.y)), glm::max(tMinTemp.z, tMaxTemp.z));

        return tMax > tMin && tMax >= 0.0f;
    }

    void testCubeIntersections(Camera* cam)
    {
        std::pair<glm::vec3, glm::vec3> rayVecs = cam->CastRay(4.0f);

        glm::vec3 rayOrigin = rayVecs.first;
        glm::vec3 rayEnd = rayVecs.second;

        std::cout << "Starting Pos: " << to_string(rayOrigin) << std::endl;
        std::cout << "Ending Pos: " << to_string(rayEnd) << std::endl;

        int chunkPosX = static_cast<int>(floor(rayOrigin.x / 16.0f));
        int chunkPosY = static_cast<int>(floor(rayOrigin.y / 16.0f));
        int chunkPosZ = static_cast<int>(floor(rayOrigin.z / 16.0f));

        //unsigned int chunkPosZ = static_cast<int>(glm::mod(rayOrigin.z, 16.0f));

        std::cout << "chunkPosX: " << chunkPosX << std::endl;
        std::cout << "chunkPosY: " << chunkPosY << std::endl;
        std::cout << "chunkPosZ: " << chunkPosZ << std::endl;

        Chunk* playerChunk = sChunk[chunkPosX][chunkPosY][chunkPosZ];
        uint8_t*** chunkBlocks = playerChunk->blocks;

        std::vector<std::pair<glm::ivec3, float>> intersectedCubes;
        for (int x = 0; x < CX; x++)
        {
            for (int y = 0; y < CY; y++)
            {
                for (int z = 0; z < CZ; z++)
                {
                    if (chunkBlocks[x][y][z])
                    {
                        // Compute the bounding box of the cube
                        glm::vec3 cubeWorldPos = {(chunkPosX * 16) + x, (chunkPosY * 16) + y, (chunkPosZ * 16) + z}; // position of the cube in world space
                        glm::vec3 cubeMin = cubeWorldPos;
                        glm::vec3 cubeMax = cubeWorldPos + 1.0f;

                        // Test the ray for intersection with the bounding box
                        float tMin, tMax;
                        if (intersectRayAABB(rayOrigin, glm::normalize(rayEnd - rayOrigin), cubeMin, cubeMax, tMin, tMax))
                        {
                            // Compute the intersection point
                            glm::vec3 intersectionPoint = rayOrigin + tMin * glm::normalize(rayEnd - rayOrigin);

                            // Compute the distance from the camera to the intersection point
                            float distance = glm::distance(rayOrigin, intersectionPoint);

                            // Compute the world position of the cube
                            //glm::ivec3 cubeCoords = glm::floor(cubePos + 0.5f); // add 0.5f to center the cube around its position
                            glm::ivec3 worldCoords = glm::ivec3(cubeWorldPos); // compute the world coordinates of the cube

                            // Add the coordinates of the cube and its distance from the camera to the list of intersected cubes
                            intersectedCubes.push_back(std::make_pair(worldCoords, distance));
                        }
                    }
                }
            }
        }

        // Sort the list of intersected cubes by distance from the camera
        std::sort(intersectedCubes.begin(), intersectedCubes.end(),
            [](const std::pair<glm::ivec3, float>& a, const std::pair<glm::ivec3, float>& b)
            {
                return a.second < b.second;
            });

        // Select the closest intersected cube
        if (!intersectedCubes.empty())
        {
            glm::ivec3 closestCube = intersectedCubes.front().first;
            std::cout << "Closest Cube World Coords: " << to_string(closestCube) << std::endl;
            // do something with closestCube...
            playerChunk->setBlock(closestCube.x % 16, closestCube.y % 16, closestCube.z % 16, 0);
        }

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
                        

                        if (y == 0 || y == 1)
                        {
                            chunk->fillWithDirt();
                        }
                        if (y == 2)
                        {
                            chunk->fillWithAir();
                        }
                        chunk->worldPosX = x;
                        chunk->worldPosZ = z;
                        sChunk[x][y][z] = chunk;
                    }
    }

    void generateSuperchunkHeightmap(float inSeed)
    {
        float yFactor = 1.0f / (CX);
        float xFactor = 1.0f / (CZ);

        float seed = inSeed;
        glm::vec2 offset = glm::vec2(seed, seed);
        glm::vec2 repeat = glm::vec2(1024.0f, 1024.0f);

        float sum = 0.0f;
        float freq = 0.0f;
        float scale = 0.0f;

        float val = 0.0f;
        float result = 0.0f;

        float a = 1.0f;
        float b = 2.0f;

        int index = 0;

        float xx = 0.0f;
        float yy = 0.0f;

        for (int x = 0; x < SCX; x++)
            for (int y = 0; y < SCY; y++)
                for (int z = 0; z < SCZ; z++)
                    if (sChunk[x][y][z])
                    {
                        for (int row = 0; row < CX; row++)
                        {
                            for (int col = 0; col < CZ; col++)
                            {
                                xx = xFactor * col;
                                yy = yFactor * row;

                                sum = 0.0f;
                                freq = a;
                                scale = b;

                                // Compute the sum for each octave
                                for (int oct = 0; oct < 4; oct++)
                                {
                                    glm::vec2 p(xx * freq, yy * freq);
                                    val = glm::perlin(p) / scale;
                                    sum += val;
                                    result = (sum + 1.0f) / 2.0f;

                                    // Store in texture buffer
                                    //std::cout << static_cast<unsigned int>(result * 16) << std::endl;
                                    heightmap[row][col] = static_cast<uint8_t>(result * 16);
                                    freq *= 2.0f;   // Double the frequency
                                    scale *= b;
                                }
                            }
                        }
                    }
    }

    
    void setChunkHeightMaps()
    {
        for (int x = 0; x < SCX; x++)
           for (int z = 0; z < SCZ; z++)
           {
               sChunk[x][SCY -1][z]->getHeightmap(heightmap);
           }
    }

    void applyHeightmaps()
    {
        for (int x = 0; x < SCX; x++)
            for (int z = 0; z < SCZ; z++)
            {
                sChunk[x][SCY - 1][z]->applyHeightmap();
            }
    }
};