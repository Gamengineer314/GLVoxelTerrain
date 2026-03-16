#include "VoxelMesh.hpp"

#include <glm/glm.hpp>

#include "Constants.hpp"

using namespace glm;


VoxelMesh::VoxelMesh(CubeNormal normal, int chunkX, int chunkZ, int startY) : 
    position(u32vec3(chunkX * CHUNK_SIZE, startY, chunkZ * CHUNK_SIZE)), 
    normal(normal),
    squaresCount(0),
    minX(CHUNK_SIZE),
    minY(CHUNK_SIZE),
    minZ(CHUNK_SIZE),
    maxX(0),
    maxY(0),
    maxZ(0) {
    position[axis(normal)] += normalPositive(normal);
}


Square VoxelMesh::add(int x, int y, int depth, int width, int height, int colorID) {
    squaresCount++;
    u32vec3 min = vec3(0, 0, 0);
    min[widthAxis(axis(normal))] += x;
    min[heightAxis(axis(normal))] += y;
    min[axis(normal)] += depth;
    u32vec3 max = min;
    max[widthAxis(axis(normal))] += width;
    max[heightAxis(axis(normal))] += height;
    if (min.x < minX) minX = min.x;
    if (min.y < minY) minY = min.y;
    if (min.z < minZ) minZ = min.z;
    if (max.x > maxX) maxX = max.x;
    if (max.y > maxY) maxY = max.y;
    if (max.z > maxZ) maxZ = max.z;
    u32vec3 pos = min + position;
    return Square(pos.x, pos.y, pos.z, width, height, normal, colorID);
}