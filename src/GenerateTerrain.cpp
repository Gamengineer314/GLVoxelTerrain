#include "GenerateTerrain.hpp"

#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>

#include "Constants.hpp"

using namespace std;

void generateHeightMap(int*& heightMap, int*& ids);
void generateIDs(int* heightMap, int* ids, vector<int>& IDs, uint32_t* IDIndexes);

#define AMPLITUDE 30
#define BLOCK_PERIOD 500
#define ID_HEIGHT 18


void generateTerrain(int*& IDs, uint32_t*& IDIndexes) {
    int* heightMap = new int[HORIZONTAL_SIZE * HORIZONTAL_SIZE];
    int* ids = new int[HORIZONTAL_SIZE * HORIZONTAL_SIZE];
    generateHeightMap(heightMap, ids);

    vector<int> IDsVec = vector<int>(); 
    IDIndexes = new uint32_t[HORIZONTAL_SIZE * HORIZONTAL_SIZE + 1];
    generateIDs(heightMap, ids, IDsVec, IDIndexes);

    delete[] heightMap;
    delete[] ids;
    IDs = new int[IDsVec.size()];
    IDIndexes[HORIZONTAL_SIZE * HORIZONTAL_SIZE] = IDsVec.size();
    memcpy(IDs, IDsVec.data(), IDsVec.size() * sizeof(int));
}

#include <iostream>
void generateHeightMap(int*& heightMap, int*& ids) {
    int index = 0;
    for (int z = 0; z < HORIZONTAL_SIZE; z++) {
        for (int x = 0; x < HORIZONTAL_SIZE; x++) {
            int height = 1 + (int)(AMPLITUDE * (sin(2 * M_PI * x / BLOCK_PERIOD) * sin(2 * M_PI * z / BLOCK_PERIOD) + 1));
            heightMap[index] = height;
            ids[index] = height / ID_HEIGHT + 1;
            index++;
        }
    }
}


void generateIDs(int* heightMap, int* ids, vector<int>& IDs, uint32_t* IDIndexes) {
    for (int chunkZ = 0; chunkZ < HORIZONTAL_CHUNKS; chunkZ++) {
        for (int chunkX = 0; chunkX < HORIZONTAL_CHUNKS; chunkX++) {
            for (int zInChunk = 0; zInChunk < CHUNK_SIZE; zInChunk++) {
                for (int xInChunk = 0; xInChunk < CHUNK_SIZE; xInChunk++) {
                    int x = chunkX * CHUNK_SIZE + xInChunk;
                    int z = chunkZ * CHUNK_SIZE + zInChunk;
                    int y = heightMap[x + z * HORIZONTAL_SIZE];
                    IDIndexes[(chunkX + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE + xInChunk + zInChunk * CHUNK_SIZE] = IDs.size();

                    // Add zeros below the block to not render invisible faces, then add the block
                    int minSurroundingY = y - 1;
                    if (x > 0) minSurroundingY = min(minSurroundingY, heightMap[x - 1 + z * HORIZONTAL_SIZE]);
                    if (x < HORIZONTAL_SIZE - 1) minSurroundingY = min(minSurroundingY, heightMap[x + 1 + z * HORIZONTAL_SIZE]);
                    if (z > 0) minSurroundingY = min(minSurroundingY, heightMap[x + (z - 1) * HORIZONTAL_SIZE]);
                    if (z < HORIZONTAL_SIZE - 1) minSurroundingY = min(minSurroundingY, heightMap[x + (z + 1) * HORIZONTAL_SIZE]);
                    for (int belowY = minSurroundingY; belowY < y; belowY++) {
                        IDs.push_back(belowY);
                        IDs.push_back(0);
                    }
                    IDs.push_back(y);
                    IDs.push_back(ids[x + z * HORIZONTAL_SIZE]);
                }
            }
        }
    }
}