#include "GenerateMesh.hpp"

#include <vector>
#include <cstdint>
#include <cmath>
#include <glm/glm.hpp>

#include "VoxelMesh.hpp"
#include "Constants.hpp"

using namespace std;
using namespace glm;

void generateBinarySolidBlocks(uint32_t chunkX, uint32_t chunkZ, int startY, int* IDs, uint32_t* IDIndexes, uint64_t* rows, bvec2* sides);
void generateXZSides(uint32_t xzIndex, bool after, int startIndex, int startY, int* IDs, uint32_t* IDIndexes, bvec2* sides);
void generateXZSides(bool after, int startIndex, bvec2* sides);
void generateBinaryPlanes(uint32_t startXZIndex, int startY, int* IDs, uint32_t* IDIndexes, uint64_t* rows, bvec2* sides, uint64_t* planes, int* idToIndex, int idCount);
void generateAxisBinaryPlanes(uint32_t axis, uint32_t startXZIndex, int startY, int* IDs, uint32_t* IDIndexes, uint64_t* rows, bvec2* sides, uint64_t* planes, int* idToIndex, int idCount);
int getID(ivec3 pos, int startXZIndex, int* IDs, uint32_t* IDIndexes);
void generateOptimizedMesh(uint32_t chunkX, uint32_t chunkZ, int startY, uint64_t* planes, int* indexToId, int idCount, vector<VoxelMesh>& meshes, vector<Square>& squares);
void generateNormalOptimizedMesh(CubeNormal normal, uint32_t chunkX, uint32_t chunkZ, int startY, uint64_t* planes, int* indexToId, int idCount, vector<VoxelMesh>& meshes, vector<Square>& squares);
void generateOptimizedPlane(CubeNormal normal, int depth, int idIndex, VoxelMesh& mesh, uint64_t* planes, int* indexToId, int idCount, vector<Square>& squares);


void generateMesh(uint32_t chunkStartX, uint32_t chunkStartZ, uint32_t chunkSizeX, uint32_t chunkSizeZ, int* IDs, uint32_t* IDIndexes, vector<VoxelMesh>& meshes, vector<Square>& squares) {
    // Find IDs in area and y range for each (x, z) chunk
    bool* containedIDs = new bool[256] { false };
    int idCount = 0;
    int* minY = new int[chunkSizeX * chunkSizeZ];
    int* maxY = new int[chunkSizeX * chunkSizeZ];
    for (uint32_t chunkZ = chunkStartZ; chunkZ < chunkStartX + chunkSizeX; chunkZ++) {
        for (uint32_t chunkX = chunkStartX; chunkX < chunkStartZ + chunkSizeZ; chunkX++) {
            int chunkMinY = VERTICAL_SIZE;
            int chunkMaxY = 0;
            for (uint32_t i = IDIndexes[(chunkX + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE]; i < IDIndexes[(chunkX + chunkZ * HORIZONTAL_CHUNKS + 1) * CHUNK_SIZE * CHUNK_SIZE]; i += 2) {
                if (IDs[i] < chunkMinY) chunkMinY = IDs[i];
                if (IDs[i] > chunkMaxY) chunkMaxY = IDs[i];
            }
            minY[chunkX - chunkStartX + (chunkZ - chunkStartZ) * chunkSizeZ] = chunkMinY;
            maxY[chunkX - chunkStartX + (chunkZ - chunkStartZ) * chunkSizeZ] = chunkMaxY;

            for (uint32_t i = 1 + IDIndexes[(chunkX + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE]; i < IDIndexes[(chunkX + chunkZ * HORIZONTAL_CHUNKS + 1) * CHUNK_SIZE * CHUNK_SIZE]; i += 2) {
                if (IDs[i] != 0 && !containedIDs[IDs[i]]) {
                    containedIDs[IDs[i]] = true;
                    idCount++;
                }
            }
        }
    }

    // Map contained IDs to smallest range possible
    int* idToIndex = new int[256];
    int* indexToId = new int[idCount];
    int index = 0;
    for (int id = 0; id < 256; id++) {
        if (containedIDs[id]) {
            idToIndex[id] = index;
            indexToId[index] = id;
            index++;
        }
    }
    delete[] containedIDs;

    // Generate all chunks
    uint64_t* rows = new uint64_t[CHUNK_SIZE * CHUNK_SIZE * 3];
    bvec2* sides = new bvec2[CHUNK_SIZE * CHUNK_SIZE * 3];
    uint64_t* planes = new uint64_t[CHUNK_SIZE * CHUNK_SIZE * idCount * 6];
    for (uint32_t chunkZ = chunkStartZ; chunkZ < chunkStartZ + chunkSizeZ; chunkZ++) {
        for (uint32_t chunkX = chunkStartX; chunkX < chunkStartX + chunkSizeX; chunkX++) {
            uint32_t startXZIndex = (chunkX + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE;
            int xzStartY = minY[chunkX - chunkStartX + (chunkZ - chunkStartZ) * chunkSizeZ];
            int sizeY = maxY[chunkX - chunkStartX + (chunkZ - chunkStartZ) * chunkSizeZ] - xzStartY + 1;
            for (int chunkY = 0; chunkY < (int)ceil((float)sizeY / CHUNK_SIZE); chunkY++) {
                // Generate one chunk
                fill(rows, rows + CHUNK_SIZE * CHUNK_SIZE * 3, 0);
                fill(sides, sides + CHUNK_SIZE * CHUNK_SIZE * 3, bvec2(false, false));
                fill(planes, planes + CHUNK_SIZE * CHUNK_SIZE * idCount * 6, 0);
                int startY = xzStartY + chunkY * CHUNK_SIZE;
                generateBinarySolidBlocks(chunkX, chunkZ, startY, IDs, IDIndexes, rows, sides);
                generateBinaryPlanes(startXZIndex, startY, IDs, IDIndexes, rows, sides, planes, idToIndex, idCount);
                generateOptimizedMesh(chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
            }
        }
    }

    delete[] minY;
    delete[] maxY;
    delete[] idToIndex;
    delete[] indexToId;
    delete[] rows;
    delete[] sides;
    delete[] planes;
}


// rows: bit rows containing 1 if the block is solid, 0 otherwise
// sides: 2 bools, first is true if the block before the row is solid, 0 otherwise, second is true if the block after the row is solid, 0 otherwise
// rows and sides contain chunkSize * chunkSize elements for each axis (x, z, y)
void generateBinarySolidBlocks(uint32_t chunkX, uint32_t chunkZ, int startY, int* IDs, uint32_t* IDIndexes, uint64_t* rows, bvec2* sides) {
    uint32_t xzIndex = (chunkX + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE;
    for (int z = 0; z < CHUNK_SIZE; z++) { // Iter chunk z
        for (int x = 0; x < CHUNK_SIZE; x++) { // Iter chunk x
            bvec2 ySide = bvec2(false, false);
            for (uint32_t i = IDIndexes[xzIndex]; i < IDIndexes[xzIndex + 1]; i += 2) { // Iter world y (only solid blocks in chunk)
                int y = IDs[i] - startY;
                if (y >= 0 && y < CHUNK_SIZE) {
                    rows[y + z * CHUNK_SIZE] |= (uint64_t)1 << x; // x
                    rows[x + z * CHUNK_SIZE + CHUNK_SIZE * CHUNK_SIZE] |= (uint64_t)1 << y; // y
                    rows[y + x * CHUNK_SIZE + 2 * CHUNK_SIZE * CHUNK_SIZE] |= (uint64_t)1 << z; // z
                }
                else if (y == -1) ySide.x = true;
                else if (y == CHUNK_SIZE) ySide.y = true;
            }
            sides[x + z * CHUNK_SIZE + CHUNK_SIZE * CHUNK_SIZE] = ySide;
            xzIndex++;
        }
    }

    // x and z sides
    if (chunkX > 0) {
        uint32_t otherStartXZIndex = (chunkX - 1 + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE;
        for (int z = 0; z < CHUNK_SIZE; z++)
            generateXZSides(otherStartXZIndex + CHUNK_SIZE - 1 + z * CHUNK_SIZE, false, z * CHUNK_SIZE, startY, IDs, IDIndexes, sides);
    }
    else {
        for (int z = 0; z < CHUNK_SIZE; z++)
            generateXZSides(false, z * CHUNK_SIZE, sides);
    }
    if (chunkX < HORIZONTAL_CHUNKS - 1) {
        uint32_t otherStartXZIndex = (chunkX + 1 + chunkZ * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE;
        for (int z = 0; z < CHUNK_SIZE; z++)
            generateXZSides(otherStartXZIndex + z * CHUNK_SIZE, true, z * CHUNK_SIZE, startY, IDs, IDIndexes, sides);
    }
    else {
        for (int z = 0; z < CHUNK_SIZE; z++)
            generateXZSides(true, z * CHUNK_SIZE, sides);
    }
    if (chunkZ > 0) {
        uint32_t otherStartXZIndex = (chunkX + (chunkZ - 1) * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE;
        for (int x = 0; x < CHUNK_SIZE; x++)
            generateXZSides(otherStartXZIndex + x + (CHUNK_SIZE - 1) * CHUNK_SIZE, false, x * CHUNK_SIZE + 2 * CHUNK_SIZE * CHUNK_SIZE, startY, IDs, IDIndexes, sides);
    }
    else {
        for (int x = 0; x < CHUNK_SIZE; x++)
            generateXZSides(false, x * CHUNK_SIZE + 2 * CHUNK_SIZE * CHUNK_SIZE, sides);
    }
    if (chunkZ < HORIZONTAL_CHUNKS - 1) {
        uint32_t otherStartXZIndex = (chunkX + (chunkZ + 1) * HORIZONTAL_CHUNKS) * CHUNK_SIZE * CHUNK_SIZE;
        for (int x = 0; x < CHUNK_SIZE; x++)
            generateXZSides(otherStartXZIndex + x, true, x * CHUNK_SIZE + 2 * CHUNK_SIZE * CHUNK_SIZE, startY, IDs, IDIndexes, sides);
    }
    else {
        for (int x = 0; x < CHUNK_SIZE; x++)
            generateXZSides(true, x * CHUNK_SIZE + 2 * CHUNK_SIZE * CHUNK_SIZE, sides);
    }
}


// Generate side row at (x, z)
void generateXZSides(uint32_t xzIndex, bool after, int startIndex, int startY, int* IDs, uint32_t* IDIndexes, bvec2* sides) {
    uint32_t i = IDIndexes[xzIndex];
    for (int y = 0; y < CHUNK_SIZE; y++) {
        if (y == IDs[i] - startY) {
            sides[startIndex + y] = bvec2(after ? sides[startIndex + y].x : true, after ? true : sides[startIndex + y].y);
            i += 2;
            if (i >= IDIndexes[xzIndex + 1]) return;
        }
    }
}

// Generate filled side row at (x, z)
void generateXZSides(bool after, int startIndex, bvec2* sides) {
    for (int y = 0; y < CHUNK_SIZE; y++) {
        sides[startIndex + y] = bvec2(after ? sides[startIndex + y].x : true, after ? true : sides[startIndex + y].y);
    }
}


// planes: 64 bits rows containing 1 if the face must be rendered, 0 otherwise
// planes contains chunkSize (rows in one plane) * chunkSize (planes in one direction) * nbrIDs * 6 (x+, z+, y+, x-, z-, y-)
void generateBinaryPlanes(uint32_t startXZIndex, int startY, int* IDs, uint32_t* IDIndexes, uint64_t* rows, bvec2* sides, uint64_t* planes, int* idToIndex, int idCount) {
    generateAxisBinaryPlanes(0, startXZIndex, startY, IDs, IDIndexes, rows, sides, planes, idToIndex, idCount);
    generateAxisBinaryPlanes(1, startXZIndex, startY, IDs, IDIndexes, rows, sides, planes, idToIndex, idCount);
    generateAxisBinaryPlanes(2, startXZIndex, startY, IDs, IDIndexes, rows, sides, planes, idToIndex, idCount);
}


// Generate binary planes for one axis
void generateAxisBinaryPlanes(uint32_t axis, uint32_t startXZIndex, int startY, int* IDs, uint32_t* IDIndexes, uint64_t* rows, bvec2* sides, uint64_t* planes, int* idToIndex, int idCount) {
    ivec3 beforeX = ivec3(0, startY, 0);
    for (int y = 0; y < CHUNK_SIZE; y++) { // Iter plane rows
        ivec3 pos = beforeX;
        for (int x = 0; x < CHUNK_SIZE; x++) { // Iter plane columns
            uint64_t row = rows[x + y * CHUNK_SIZE + axis * CHUNK_SIZE * CHUNK_SIZE];
            bvec2 side = sides[x + y * CHUNK_SIZE + axis * CHUNK_SIZE * CHUNK_SIZE];

            // Find faces to render in positive direction and add them to planes
            uint64_t shiftedRow = row >> 1;
            if (side.y) shiftedRow |= (uint64_t)1 << 63;
            uint64_t faceRow = row & ~shiftedRow;
            while (faceRow != 0) {
                int depth = __builtin_ctzll(faceRow);
                faceRow &= ~((uint64_t)1 << depth);
                ivec3 posDepth = pos;
                posDepth[axis] += depth;
                int id = getID(posDepth, startXZIndex, IDs, IDIndexes);
                if (id != 0) {
                    planes[y + depth * CHUNK_SIZE + idToIndex[id] * CHUNK_SIZE * CHUNK_SIZE + 2 * axis * CHUNK_SIZE * CHUNK_SIZE * idCount]
                        |= (uint64_t)1 << x;
                }
            }

            // Find faces to render in negative direction and add them to planes
            shiftedRow = row << 1;
            if (side.x) shiftedRow |= 1;
            faceRow = row & ~shiftedRow;
            while (faceRow != 0) {
                int depth = __builtin_ctzll(faceRow);
                faceRow &= ~((uint64_t)1 << depth);
                ivec3 posDepth = pos;
                posDepth[axis] += depth;
                int id = getID(posDepth, startXZIndex, IDs, IDIndexes);
                if (id != 0) {
                    planes[y + depth * CHUNK_SIZE + idToIndex[id] * CHUNK_SIZE * CHUNK_SIZE + (2 * axis + 1) * CHUNK_SIZE * CHUNK_SIZE * idCount]
                        |= (uint64_t)1 << x;
                }
            }

            pos[WIDTH_AXIS(axis)]++;
        }
        beforeX[HEIGHT_AXIS(axis)]++;
    }
}


int getID(ivec3 pos, int startXZIndex, int* IDs, uint32_t* IDIndexes) {
    for (uint32_t i = IDIndexes[startXZIndex + pos.x + pos.z * CHUNK_SIZE]; i < IDIndexes[startXZIndex + pos.x + pos.z * CHUNK_SIZE + 1]; i += 2) {
        if (IDs[i] == pos.y) return IDs[i + 1];
    }
    return 0; // Shouldn't happen
}


void generateOptimizedMesh(uint32_t chunkX, uint32_t chunkZ, int startY, uint64_t* planes, int* indexToId, int idCount, vector<VoxelMesh>& meshes, vector<Square>& squares) {
    generateNormalOptimizedMesh(CubeNormal::xPositive, chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
    generateNormalOptimizedMesh(CubeNormal::xNegative, chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
    generateNormalOptimizedMesh(CubeNormal::yPositive, chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
    generateNormalOptimizedMesh(CubeNormal::yNegative, chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
    generateNormalOptimizedMesh(CubeNormal::zPositive, chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
    generateNormalOptimizedMesh(CubeNormal::zNegative, chunkX, chunkZ, startY, planes, indexToId, idCount, meshes, squares);
}


void generateNormalOptimizedMesh(CubeNormal normal, uint32_t chunkX, uint32_t chunkZ, int startY, uint64_t* planes, int* indexToId, int idCount, vector<VoxelMesh>& meshes, vector<Square>& squares) {
    VoxelMesh mesh = VoxelMesh(normal, chunkX, chunkZ, startY);
    for (int i = 0; i < idCount; i++) {
        for (int depth = 0; depth < CHUNK_SIZE; depth++) {
            generateOptimizedPlane(normal, depth, i, mesh, planes, indexToId, idCount, squares);
        }
    }
    if (mesh.squaresCount != 0) meshes.push_back(mesh);
}


void generateOptimizedPlane(CubeNormal normal, int depth, int idIndex, VoxelMesh& mesh, uint64_t* planes, int* indexToId, int idCount, vector<Square>& squares) {
    int startIndex =
        (int)normal * CHUNK_SIZE * CHUNK_SIZE * idCount
        + idIndex * CHUNK_SIZE * CHUNK_SIZE
        + depth * CHUNK_SIZE;
    for (int y = 0; y < CHUNK_SIZE; y++) { // Iter plane rows
        uint64_t row = planes[startIndex + y];
        int x = row == 0 ? 64 : __builtin_ctzll(row);
        row >>= x;
        while (x < CHUNK_SIZE) {
            // Expand in x
            int width = ~row == 0 ? 64 : __builtin_ctzll(~row);
            uint64_t checkMask = (row << (64 - width)) >> (64 - width - x);
            uint64_t deleteMask = ~checkMask;
            row >>= width;

            // Expand in y
            int height = 1;
            while (y + height < CHUNK_SIZE) {
                if ((planes[startIndex + y + height] & checkMask) != checkMask) break;
                planes[startIndex + y + height] &= deleteMask;
                height++;
            }

            // Add the rectangle
            squares.push_back(mesh.add(x, y, depth, width, height, indexToId[idIndex]));
            x += width;

            // Skip zeros
            int skip = row == 0 ? 64 : __builtin_ctzll(row);
            x += skip;
            row >>= skip;
        }
    }
}