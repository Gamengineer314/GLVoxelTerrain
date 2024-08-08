#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include <cstdint>
#include <glm/glm.hpp>

using namespace glm;

class VoxelMesh;
class Square;
class MeshData;



enum class CubeNormal {
    xPositive = 0,
    xNegative = 1,
    yPositive = 2,
    yNegative = 3,
    zPositive = 4,
    zNegative = 5
};


// x: 0, y: 1, z: 2
#define AXIS(normal) ((uint32_t)(normal) >> 1)
// x: 1, y: 0, z: 1
#define WIDTH_AXIS(axis) ((uint32_t)1 & ~(uint32_t)(axis))
// x: 2, y: 2, z: 0
#define HEIGHT_AXIS(axis) ((uint32_t)2 & ~(uint32_t)(axis))
// +: 1, -: 0
#define NORMAL_POSITIVE(normal) (~(uint32_t)(normal) & (uint32_t)1)
// +: 0, -: 1
#define NORMAL_NEGATIVE(normal) ((uint32_t)(normal) & (uint32_t)1)
// +: 1, -: -1
#define NORMAL_SIGN(normal) ((int)1 | -(int)((uint32_t)(normal) & (uint32_t)1))



// Mesh with all rectangles with the same normal and in the same chunk
class VoxelMesh {

public:
    u32vec3 position;
    uint32_t normal;
    uint32_t squaresCount;

    /**
     * @brief Create a new voxel mesh
     * @param normal Normal of all rectangles
     * @param chunkX x index of the chunk the mesh is in
     * @param chunkZ z index of the chunk the mesh is in
     * @param startY y index of the chunk the mesh is in
    **/
    VoxelMesh(CubeNormal normal, int chunkX, int chunkZ, int startY);

    /**
     * @brief Add a new rectangle to the mesh
     * @param x x start index of the rectangle in the plane
     * @param y y start index of the rectangle in the plane
     * @param depth Index of the plane
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     * @param colorID Color ID of the rectangle
     * @return The square that was added (must be stored in a seperate container)
    **/
    Square add(int x, int y, int depth, int width, int height, int colorID);

    vec3 center();

    vec3 size();

private:
    uint32_t minX;
    uint32_t minY;
    uint32_t minZ;
    uint32_t maxX;
    uint32_t maxY;
    uint32_t maxZ;
    
};



class Square {

public:
    Square(uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, uint32_t normal, uint32_t colorID);

private:
    uint32_t data1; // x (13b), z (13b)
    uint32_t data2; // y (9b), width (6b), height (6b), normal (3b), color (8b)

};



class MeshData {

public:
    MeshData(vec3 center, vec3 size, uint32_t normal, uint32_t squareCount, uint32_t startSquare);
    MeshData(VoxelMesh& mesh, uint32_t startSquare) : MeshData(mesh.center(), mesh.size(), mesh.normal, mesh.squaresCount, startSquare) {};

private:
    vec3 center;
    vec3 size;
    uint32_t data1; // normal (3b), squareCount (29b)
    uint32_t data2; // startSquare (32b)

};



#endif // VOXEL_MESH_H