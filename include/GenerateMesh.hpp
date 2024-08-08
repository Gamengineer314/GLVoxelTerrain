#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <vector>
#include <cstdint>

#include "VoxelMesh.hpp"

using namespace std;

// IDs contains all block rows one after the other.
// A row contains all blocks for an (x, z) coordinate in ascending order.
// Even indices : y coordinates, odd indices : color ids.
// Index of the start of each row in IDs : IDIndexes.
// Index of a row in IDIndexes : (chunkX + chunkZ * horizontalChunks) + xInChunk + zInChunk * chunkSize
// ID 0 : invisible block used to not render faces arround it.

/**
 * @brief 
 * Generate an optimized mesh (greedy meshing) for the terrain from block IDs. 
 * The mesh will be split in chunks and different face orientations.
 * @param chunkStartX x start (in chunks) of the part of IDs to render
 * @param chunkStartZ z start (in chunks) of the part of IDs to render
 * @param chunkSizeX x size (in chunks) of the part of IDs to render
 * @param chunkSizeZ z size (in chunks) of the part of IDs to render
 * @param IDs Block IDs
 * @param IDIndexes Start index for each (x, z) in IDs
 * @param meshes Vector to add output meshes to
 * @param squares Vector to add output squares to
**/
void generateMesh(uint32_t chunkStartX, uint32_t chunkStartZ, uint32_t chunkSizeX, uint32_t chunkSizeZ, int* IDs, uint32_t* IDIndexes, vector<VoxelMesh>& meshes, vector<Square>& squares);

#endif // MESH_GENERATOR_H