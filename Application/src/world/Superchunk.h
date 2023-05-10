#pragma once
#define SCX 16
#define SCY 5
#define SCZ 16

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ChunkGeneration.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_map.hpp>

#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <random>

struct Superchunk
{
    std::unique_ptr<Chunk> sChunk[SCX][SCY][SCZ];
    std::unique_ptr<int8_t* []> heightmap;

    int worldSeed;
    int heightmapStartingChunk;

    std::shared_ptr<std::unordered_map<uint8_t, uint8_t>> twoSidedBlocks;
    std::shared_ptr<std::unordered_map<uint8_t, std::pair<uint8_t, uint8_t>>> threeSidedBlocks;
    Superchunk()
    {
        twoSidedBlocks = std::make_shared<std::unordered_map<uint8_t, uint8_t>>();
        threeSidedBlocks = std::make_shared<std::unordered_map<uint8_t, std::pair<uint8_t, uint8_t>>>();

        // Allocate memory for sChunk using std::make_unique
        for (int x = 0; x < SCX; x++) {
            for (int y = 0; y < SCY; y++) {
                for (int z = 0; z < SCZ; z++) {

                    sChunk[x][y][z] = std::make_unique<Chunk>();

                    if (y == 0 || y == 1)
                    {
                        sChunk[x][y][z]->fillWithBlock(STONE);
                    }
                    if (y > 1)
                    {
                        sChunk[x][y][z]->fillWithAir();
                    }

                    sChunk[x][y][z]->worldPosX = x;
                    sChunk[x][y][z]->worldPosZ = z;

                    sChunk[x][y][z]->twoSidedBlocks = twoSidedBlocks;
                    sChunk[x][y][z]->threeSidedBlocks = threeSidedBlocks;
                }
            }
        }


        // Allocate memory for heightmap using std::make_unique
        heightmap = std::make_unique<int8_t* []>(SCX * 16);
        for (int i = 0; i < (SCX * 16); i++) {
            heightmap[i] = new int8_t[SCZ * 16];
        }

        twoSidedBlocks->insert({ OAKLOG, OAKLOGTOP });
        twoSidedBlocks->insert({ MARBLESIDE, MARBLETOP });
        twoSidedBlocks->insert({ MELONTOP, MELONSIDE });

        threeSidedBlocks->insert({ GRASS, std::make_pair(GRASSSIDE, DIRT) });
        threeSidedBlocks->insert({ CACTUSTOP, std::make_pair(CACTUSSIDE, CACTUSBOTTOM) });
        threeSidedBlocks->insert({ PUMPKINTOP, std::make_pair(JACKOLANTERNLIT, PUMPKINSIDE) });

        worldSeed = 0;
        heightmapStartingChunk = 2;


    }

    template <class Archive>
    void serialize(cereal::BinaryOutputArchive& archive, Superchunk& superchunk)
    {
        archive(superchunk.sChunk, superchunk.heightmap, superchunk.heightmapStartingChunk,
            superchunk.twoSidedBlocks, superchunk.threeSidedBlocks);
    }

    void generateSuperchunk()
    {
        generateSuperchunkHeightmap(static_cast<float>(worldSeed));
        applyHeightmap(DIRT, GRASS);
        placeTreesInWorld();
    }


    void setSeed(int seed)
    {
        worldSeed = seed;
    }


    enum CubeFace {
        PositiveX,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ,
    };

    bool intersectRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& tMin, float& tMax)
    {
        glm::vec3 invRayDirection = 1.0f / rayDirection;
        glm::vec3 tMinTemp = (aabbMin - rayOrigin) * invRayDirection;
        glm::vec3 tMaxTemp = (aabbMax - rayOrigin) * invRayDirection;

        tMin = glm::max(glm::max(glm::min(tMinTemp.x, tMaxTemp.x), glm::min(tMinTemp.y, tMaxTemp.y)), glm::min(tMinTemp.z, tMaxTemp.z));
        tMax = glm::min(glm::min(glm::max(tMinTemp.x, tMaxTemp.x), glm::max(tMinTemp.y, tMaxTemp.y)), glm::max(tMinTemp.z, tMaxTemp.z));

        return tMax > tMin && tMax >= 0.0f;
    }

    void breakBlock(Camera* cam)
    {
        std::pair<glm::vec3, glm::vec3> rayVecs = cam->CastRay(2.0f);

        glm::vec3 rayOrigin = rayVecs.first;
        glm::vec3 rayEnd = rayVecs.second;

        //std::cout << "Starting Pos: " << to_string(rayOrigin) << std::endl;
        //std::cout << "Ending Pos: " << to_string(rayEnd) << std::endl;

        std::vector<std::pair<glm::ivec3, float>> intersectedCubes;

        glm::ivec3 originChunkWorldPos(static_cast<int>(floor(rayOrigin.x / 16.0f)), static_cast<int>(floor(rayOrigin.y / 16.0f)), static_cast<int>(floor(rayOrigin.z / 16.0f)));
        glm::ivec3 endChunkWorldPos(static_cast<int>(floor(rayEnd.x / 16.0f)), static_cast<int>(floor(rayEnd.y / 16.0f)), static_cast<int>(floor(rayEnd.z / 16.0f)));

        if (originChunkWorldPos != endChunkWorldPos)
        {
            glm::ivec3 pVector = originChunkWorldPos - endChunkWorldPos;

            searchChunkForCube(intersectedCubes, originChunkWorldPos, rayOrigin, rayEnd);
            searchChunkForCube(intersectedCubes, endChunkWorldPos, rayOrigin, rayEnd);
        }
        else
        {
            searchChunkForCube(intersectedCubes, originChunkWorldPos, rayOrigin, rayEnd);
        }
        //unsigned int chunkPosZ = static_cast<int>(glm::mod(rayOrigin.z, 16.0f));

        //std::cout << "originChunkWorldPosX: " << originChunkWorldPos.x << std::endl;
        //std::cout << "originChunkWorldPosY: " << originChunkWorldPos.y << std::endl;
        //std::cout << "originChunkWorldPosZ: " << originChunkWorldPos.z << std::endl;

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
            //std::cout << "Closest Cube World Coords: " << to_string(closestCube) << std::endl;

            setWorldBlock(closestCube.x, closestCube.y, closestCube.z, 0);
        }
    }

    void searchChunkForCube(std::vector<std::pair<glm::ivec3, float>>& intersectedCubes,
        glm::ivec3 chunkWorldPos, glm::vec3 rayOrigin, glm::vec3 rayEnd)
    {
        Chunk* playerChunk = sChunk[chunkWorldPos.x][chunkWorldPos.y][chunkWorldPos.z].get();
        auto& chunkBlocks = playerChunk->blocks;

        for (int x = 0; x < CX; x++)
        {
            for (int y = 0; y < CY; y++)
            {
                for (int z = 0; z < CZ; z++)
                {
                    if (chunkBlocks[x][y][z])
                    {
                        // Compute the bounding box of the cube
                        glm::vec3 cubeWorldPos = { (chunkWorldPos.x * 16) + x, (chunkWorldPos.y * 16) + y, (chunkWorldPos.z * 16) + z }; // position of the cube in world space
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
    }

    void placeBlock(Camera* cam, uint8_t selectedBlockType)
    {
        std::pair<glm::vec3, glm::vec3> rayVecs = cam->CastRay(2.0f);

        glm::vec3 rayOrigin = rayVecs.first;
        glm::vec3 rayEnd = rayVecs.second;

        //std::cout << "Starting Pos: " << to_string(rayOrigin) << std::endl;
        //std::cout << "Ending Pos: " << to_string(rayEnd) << std::endl;

        std::vector<std::pair<glm::ivec3, float>> intersectedCubes;

        glm::ivec3 originChunkWorldPos(static_cast<int>(floor(rayOrigin.x / 16.0f)), static_cast<int>(floor(rayOrigin.y / 16.0f)), static_cast<int>(floor(rayOrigin.z / 16.0f)));
        glm::ivec3 endChunkWorldPos(static_cast<int>(floor(rayEnd.x / 16.0f)), static_cast<int>(floor(rayEnd.y / 16.0f)), static_cast<int>(floor(rayEnd.z / 16.0f)));

        if (originChunkWorldPos != endChunkWorldPos)
        {
            searchChunkForCube(intersectedCubes, originChunkWorldPos, rayOrigin, rayEnd);
            searchChunkForCube(intersectedCubes, endChunkWorldPos, rayOrigin, rayEnd);
        }
        else
        {
            searchChunkForCube(intersectedCubes, originChunkWorldPos, rayOrigin, rayEnd);
        }
        //unsigned int chunkPosZ = static_cast<int>(glm::mod(rayOrigin.z, 16.0f));

        //std::cout << "originChunkWorldPosX: " << originChunkWorldPos.x << std::endl;
        //std::cout << "originChunkWorldPosY: " << originChunkWorldPos.y << std::endl;
        //std::cout << "originChunkWorldPosZ: " << originChunkWorldPos.z << std::endl;

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

            std::pair<glm::ivec3, CubeFace> closestCube_closestFace = getClosestCubeFace(intersectedCubes.front(), rayVecs);

            CubeFace closestFace = closestCube_closestFace.second;

            uint8_t blockPlaceType = selectedBlockType;

            switch (closestFace) {
            case PositiveX:
                setWorldBlock(closestCube.x + 1, closestCube.y, closestCube.z, blockPlaceType);
                break;
            case NegativeX:
                setWorldBlock(closestCube.x - 1, closestCube.y, closestCube.z, blockPlaceType);
                break;
            case PositiveY:
                setWorldBlock(closestCube.x, closestCube.y + 1, closestCube.z, blockPlaceType);
                break;
            case NegativeY:
                setWorldBlock(closestCube.x, closestCube.y - 1, closestCube.z, blockPlaceType);
                break;
            case PositiveZ:
                setWorldBlock(closestCube.x, closestCube.y, closestCube.z + 1, blockPlaceType);
                break;
            case NegativeZ:
                setWorldBlock(closestCube.x, closestCube.y, closestCube.z - 1, blockPlaceType);
                break;
            default:
                std::cout << "Invalid closest face." << std::endl;
                break;
            }



            // do something with closestCube...
            //playerChunk->setBlock(closestCube.x % 16, closestCube.y % 16, closestCube.z % 16, 0);
        }

    }

    std::pair<glm::ivec3, CubeFace> getClosestCubeFace(std::pair<glm::ivec3, float>& closestCube, std::pair<glm::vec3, glm::vec3> rayCoords)
    {
        glm::vec3 rayOrigin = rayCoords.first;
        glm::vec3 rayEnd = rayCoords.second;

        //std::cout << "Closest Cube World Coords: " << to_string(closestCube) << std::endl;
        // do something with closestCube...

        // Compute the distance between the intersection point and each face of the cube
        glm::vec3 intersectionPoint = rayOrigin + closestCube.second * glm::normalize(rayEnd - rayOrigin);
        glm::vec3 cubeMin = glm::vec3(closestCube.first);
        glm::vec3 cubeMax = glm::vec3(closestCube.first) + 1.0f;
        glm::vec3 faceDistances[6] = {
            glm::vec3(cubeMax.x, intersectionPoint.y, intersectionPoint.z) - intersectionPoint, // Positive X
            glm::vec3(cubeMin.x, intersectionPoint.y, intersectionPoint.z) - intersectionPoint, // Negative X
            glm::vec3(intersectionPoint.x, cubeMax.y, intersectionPoint.z) - intersectionPoint, // Positive Y
            glm::vec3(intersectionPoint.x, cubeMin.y, intersectionPoint.z) - intersectionPoint, // Negative Y
            glm::vec3(intersectionPoint.x, intersectionPoint.y, cubeMax.z) - intersectionPoint, // Positive Z
            glm::vec3(intersectionPoint.x, intersectionPoint.y, cubeMin.z) - intersectionPoint, // Negative Z
        };

        // Find the face with the smallest distance
        float minDistance = std::numeric_limits<float>::max();
        CubeFace closestFace = PositiveX;
        for (int i = 0; i < 6; i++) {
            float distance = glm::length(faceDistances[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestFace = static_cast<CubeFace>(i);
            }
        }

        return std::make_pair(closestCube.first, closestFace);

        //return std::make_pair(glm::ivec3(0), PositiveX); // return a default value if no intersection is found
    }


    uint8_t getWorldBlock(int x, int y, int z) {
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

    void setWorldBlock(int x, int y, int z, uint8_t type) {
        int cx = x / CX;
        int cy = y / CY;
        int cz = z / CZ;

        x %= CX;
        y %= CY;
        z %= CZ;

        //if (!sChunk[cx][cy][cz])
            //sChunk[cx][cy][cz] = new Chunk();

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

    /*void fillSuperchunk()
    {
        for (int x = 0; x < SCX; x++)
            for (int y = 0; y < SCY; y++)
                for (int z = 0; z < SCZ; z++)
                    if (!sChunk[x][y][z])
                    {
                        Chunk* chunk = new Chunk();


                        if (y == 0 || y == 1)
                        {
                            chunk->fillWithBlock(STONE);
                        }
                        if (y > 1)
                        {
                            chunk->fillWithAir();
                        }

                        chunk->worldPosX = x;
                        chunk->worldPosZ = z;

                        chunk->twoSidedBlocks = twoSidedBlocks;
                        chunk->threeSidedBlocks = threeSidedBlocks;

                        sChunk[x][y][z] = chunk;
                    }
    }*/

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
        {
            for (int z = 0; z < SCZ; z++)
            {
                for (int row = 0; row < CX; row++)
                {
                    for (int col = 0; col < CZ; col++)
                    {
                        xx = xFactor * ((x * 16) + col);
                        yy = yFactor * ((z * 16) + row);

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
                            heightmap[(x * 16) + col][(z * 16) + row] = static_cast<uint8_t>(result * 16);
                            freq *= 2.0f;   // Double the frequency
                            scale *= b;
                        }
                    }
                }
            }
        }

    }


    /*void setChunkHeightMaps()
    {
        for (int x = 0; x < SCX; x++)
           for (int z = 0; z < SCZ; z++)
           {
               if (SCY >= 3)
               {
                   sChunk[x][heightmapStartingChunk][z]->getHeightmap(heightmap.get());
               }
               else
               {
                   heightmapStartingChunk = 1;
                   sChunk[x][heightmapStartingChunk][z]->getHeightmap(heightmap.get());
               }
           }
    }

    void applyHeightmaps()
    {
        for (int x = 0; x < SCX; x++)
            for (int z = 0; z < SCZ; z++)
            {
                if (SCY >= 3)
                {
                    sChunk[x][2][z]->applyHeightmap();
                }
                else
                {
                    sChunk[x][SCY - 1][z]->applyHeightmap();
                }
            }
    }*/

    void applyHeightmap(uint8_t groundType, uint8_t surfaceType)
    {
        int heightmapOffset = heightmapStartingChunk * CY;

        for (int sx = 0; sx < SCX; sx++)
        {
            for (int sz = 0; sz < SCZ; sz++)
            {
                for (int y = heightmapOffset; y < heightmapOffset + CY; y++)
                {
                    for (int x = 0; x < CX; x++)
                    {
                        for (int z = 0; z < CZ; z++)
                        {
                            //std::cout << heightmap[x][z] << std::endl;

                            int worldX = (sx * CX) + x;
                            int worldZ = (sz * CZ) + z;

                            if (y < (heightmapOffset + heightmap[worldX][worldZ]))
                            {
                                setWorldBlock(worldX, y, worldZ, groundType);
                            }
                            if (y == (heightmapOffset + heightmap[worldX][worldZ]))
                            {
                                setWorldBlock(worldX, y, worldZ, surfaceType);
                            }
                        }
                    }
                }
            }
        }
    }

    void placeTreesInWorld()
    {
        int lowRange = 10;
        int highRangeX = (SCX * 16) - 20;
        int highRangeZ = (SCZ * 16) - 20;

        int randX = 0;
        int randZ = 0;

        srand(static_cast<unsigned int>(worldSeed));

        std::vector<glm::ivec2> treeCoords;

        for (int x = 0; x < (SCX); x++)
            for (int z = 0; z < (SCZ); z++)
            {
                randX = (rand() % highRangeX) + lowRange;
                randZ = (rand() % highRangeZ) + lowRange;

                generateTree(glm::ivec2(randX, randZ));
                //treeCoords.push_back(glm::ivec2(randX, randZ));
            }
    }

    void generateTree(glm::ivec2 treePos)
    {

        // Determine the position of the trunk base
        int x = treePos.x;
        int z = treePos.y;
        int y = (heightmapStartingChunk * 16) + heightmap[x][z] + 1;

        int randTreeHeight = (rand() % 4) + 4;

        // Create the trunk
        for (int i = y; i < y + randTreeHeight; i++) {
            setWorldBlock(x, i, z, OAKLOG);
        }

        setWorldBlock(x, y + randTreeHeight, z, LEAVES);

        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(0, 1);

        // Create the leaves
        for (int dy = y + 2; dy < y + 6; dy++) {
            for (int dx = x - 2; dx <= x + 2; dx++) {
                for (int dz = z - 2; dz <= z + 2; dz++) {
                    if (dx != x || dz != z || dy != y + 5) { // Prevent leaves from replacing trunk or appearing on the top trunk block
                        float distanceToTrunk = glm::distance(glm::vec3(x, dy, z), glm::vec3(dx, dy, dz));

                        // Create leaves with a random chance, closer to the trunk means a higher chance
                        if (dist(rng) < (1.0f - (distanceToTrunk / 2.5f))) {
                            if (!(getWorldBlock(dx, dy, dz) == OAKLOG))
                                setWorldBlock(dx, dy, dz, LEAVES);
                        }
                    }
                }
            }
        }
    }

    void addWaterToWorld()
    {
        int heightmapOffset = heightmapStartingChunk * CY;

        int waterLevel = heightmapOffset + 7;

        for (int sx = 0; sx < SCX; sx++)
        {
            for (int sz = 0; sz < SCZ; sz++)
            {
                for (int y = heightmapOffset; y < heightmapOffset + CY; y++)
                {
                    for (int x = 0; x < CX; x++)
                    {
                        for (int z = 0; z < CZ; z++)
                        {
                            //std::cout << heightmap[x][z] << std::endl;

                            int worldX = (sx * CX) + x;
                            int worldZ = (sz * CZ) + z;

                            if (y > (heightmapOffset + heightmap[worldX][worldZ]) && y < waterLevel)
                            {
                                setWorldBlock(worldX, y, worldZ, LAVA);
                            }
                        }
                    }
                }
            }
        }
    }

};