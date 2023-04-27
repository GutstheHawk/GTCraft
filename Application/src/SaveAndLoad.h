#pragma once

#include <fstream>
#include <iostream>
#include "Superchunk.h"

static void saveWorld(const Superchunk& superchunk, const std::string& filename)
{
    // Open a file stream for writing
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Write the sChunk array to the file
    for (int x = 0; x < SCX; x++) {
        for (int y = 0; y < SCY; y++) {
            for (int z = 0; z < SCZ; z++) {
                Chunk* chunk = superchunk.sChunk[x][y][z];
                if (chunk != nullptr) {
                    // Write the chunk data to the file
                    outFile.write(reinterpret_cast<const char*>(chunk), sizeof(Chunk));
                    for (int x = 0; x < CX; x++) {
                        for (int y = 0; y < CY; y++) {
                            outFile.write(reinterpret_cast<const char*>(chunk->blocks[x][y]), CZ * sizeof(uint8_t));
                        }
                    }

                    // Write the rest of the data to the file
                    outFile.write(reinterpret_cast<const char*>(&chunk->elements), sizeof(int));
                    outFile.write(reinterpret_cast<const char*>(&chunk->changed), sizeof(bool));
                    outFile.write(reinterpret_cast<const char*>(&chunk->worldPosX), sizeof(int));
                    outFile.write(reinterpret_cast<const char*>(&chunk->worldPosZ), sizeof(int));
                    outFile.write(reinterpret_cast<const char*>(&chunk->heightmap), CX * CZ * sizeof(int));

                    // Write the vertex data to the file
                    outFile.write(reinterpret_cast<const char*>(chunk->vertexes), chunk->elements * sizeof(Vertex));
                }
            }
        }
    }

    // Write the heightmap array to the file
    for (int i = 0; i < (SCX * 16) * (SCZ * 16); i++) {
        int8_t value = superchunk.heightmap[i % (SCX * 16)][i / (SCX * 16)];
        outFile.write(reinterpret_cast<const char*>(&value), sizeof(int8_t));
    }

    // Write the rest of the data to the file
    outFile.write(reinterpret_cast<const char*>(&superchunk.heightmapStartingChunk), sizeof(int));
    int twoSidedSize = superchunk.twoSidedBlocks.size();
    int threeSidedSize = superchunk.threeSidedBlocks.size();
    outFile.write(reinterpret_cast<const char*>(&twoSidedSize), sizeof(int));
    outFile.write(reinterpret_cast<const char*>(&threeSidedSize), sizeof(int));

    // Write the key-value pairs of the two-sided blocks map to the file
    for (const auto& pair : superchunk.twoSidedBlocks) {
        outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(uint8_t));
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(uint8_t));
    }

    // Write the key-value pairs of the three-sided blocks map to the file
    for (const auto& pair : superchunk.threeSidedBlocks) {
        outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(uint8_t));
        outFile.write(reinterpret_cast<const char*>(&pair.second.first), sizeof(uint8_t));
        outFile.write(reinterpret_cast<const char*>(&pair.second.second), sizeof(uint8_t));
    }

    // Close the file stream
    outFile.close();
}

static Superchunk* loadWorld(const std::string& filename)
{
    Superchunk superchunk;

    // Open a file stream for reading
    std::ifstream inFile(filename, std::ios::in | std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return &superchunk;
    }

    // Read the sChunk array from the file
    for (int x = 0; x < SCX; x++) {
        for (int y = 0; y < SCY; y++) {
            for (int z = 0; z < SCZ; z++) {
                Chunk* chunk = new Chunk;
                inFile.read(reinterpret_cast<char*>(chunk), sizeof(Chunk));
                for (int x = 0; x < CX; x++) {
                    for (int y = 0; y < CY; y++) {
                        //chunk->blocks[x][y] = new uint8_t[CZ];
                        inFile.read(reinterpret_cast<char*>(chunk->blocks[x][y]), CZ * sizeof(uint8_t));
                    }
                }
                inFile.read(reinterpret_cast<char*>(&chunk->elements), sizeof(int));
                inFile.read(reinterpret_cast<char*>(&chunk->changed), sizeof(bool));
                inFile.read(reinterpret_cast<char*>(&chunk->worldPosX), sizeof(int));
                inFile.read(reinterpret_cast<char*>(&chunk->worldPosZ), sizeof(int));
                inFile.read(reinterpret_cast<char*>(&chunk->heightmap), CX * CZ * sizeof(int));
                //chunk->vertexes = new Vertex[CX * CY * CZ * 6 * 6];
                inFile.read(reinterpret_cast<char*>(chunk->vertexes), chunk->elements * sizeof(Vertex));

                superchunk.sChunk[x][y][z] = chunk;
            }
        }
    }

    // Read the heightmap array from the file
    for (int i = 0; i < (SCX * 16) * (SCZ * 16); i++) {
        int8_t value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(int8_t));
        superchunk.heightmap[i % (SCX * 16)][i / (SCX * 16)] = value;
    }

    // Read the rest of the data from the file
    inFile.read(reinterpret_cast<char*>(&superchunk.heightmapStartingChunk), sizeof(int));
    int twoSidedSize, threeSidedSize;
    inFile.read(reinterpret_cast<char*>(&twoSidedSize), sizeof(int));
    inFile.read(reinterpret_cast<char*>(&threeSidedSize), sizeof(int));

    // Read the key-value pairs of the two-sided blocks map from the file
    for (int i = 0; i < twoSidedSize; i++) {
        uint8_t key, value;
        inFile.read(reinterpret_cast<char*>(&key), sizeof(uint8_t));
        inFile.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        superchunk.twoSidedBlocks[key] = value;
    }

    // Read the key-value pairs of the three-sided blocks map from the file
    for (int i = 0; i < threeSidedSize; i++) {
        uint8_t key, value1, value2;
        inFile.read(reinterpret_cast<char*>(&key), sizeof(uint8_t));
        inFile.read(reinterpret_cast<char*>(&value1), sizeof(uint8_t));
        inFile.read(reinterpret_cast<char*>(&value2), sizeof(uint8_t));
        superchunk.threeSidedBlocks[key] = std::make_pair(value1, value2);
    }

    // Close the file stream
    inFile.close();

    return &superchunk;
}

static Superchunk* CreateOrLoadWorld()
{
    std::cout << "Would you like to load your previous world or create a new one [Type load or new]";
    //std::string = std::cin;
}

