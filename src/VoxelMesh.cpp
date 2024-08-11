#include "VoxelMesh.hpp"

#include <glm/glm.hpp>

#include "Constants.hpp"

using namespace glm;



VoxelMesh::VoxelMesh(CubeNormal normal, int chunkX, int chunkZ, int startY) : 
    position(u32vec3(chunkX * CHUNK_SIZE, startY, chunkZ * CHUNK_SIZE)), 
    normal((uint32_t)normal),
    squaresCount(0),
    minX(CHUNK_SIZE),
    minY(CHUNK_SIZE),
    minZ(CHUNK_SIZE),
    maxX(0),
    maxY(0),
    maxZ(0) {
}



Square VoxelMesh::add(int x, int y, int depth, int width, int height, int colorID) {
    squaresCount++;
    u32vec3 min = vec3(0, 0, 0);
    min[WIDTH_AXIS(AXIS(normal))] += x;
    min[HEIGHT_AXIS(AXIS(normal))] += y;
    min[AXIS(normal)] += depth + NORMAL_POSITIVE(normal);
    u32vec3 max = min;
    max[WIDTH_AXIS(AXIS(normal))] += width;
    max[HEIGHT_AXIS(AXIS(normal))] += height;
    if (min.x < minX) minX = min.x;
    if (min.y < minY) minY = min.y;
    if (min.z < minZ) minZ = min.z;
    if (max.x > maxX) maxX = max.x;
    if (max.y > maxY) maxY = max.y;
    if (max.z > maxZ) maxZ = max.z;
    u32vec3 pos = min + position;
    return Square(pos.x, pos.y, pos.z, width, height, normal, colorID);
}


vec3 VoxelMesh::center() {
    return (vec3)position + vec3(minX + maxX, minY + maxY, minZ + maxZ) / 2.0f;
}


vec3 VoxelMesh::size() {
    return vec3(maxX - minX, maxY - minY, maxZ - minZ) / 2.0f;
}



Square::Square(uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, uint32_t normal, uint32_t colorID) :
    data1(x | (z << 13)),
    data2(y | ((w - 1) << 9) | ((h - 1) << 15) | (normal << 21) | (colorID << 24)) {
}



MeshData::MeshData(vec3 center, vec3 size, uint32_t normal, uint32_t squareCount, uint32_t startSquare) :
    center(center),
    data1(normal | (squareCount << 3)),
    size(size),
    data2(startSquare) {
}