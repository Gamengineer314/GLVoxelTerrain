#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include <cstdint>
#include <glm/glm.hpp>



enum class CubeNormal {
    xPositive = 0,
    xNegative = 1,
    yPositive = 2,
    yNegative = 3,
    zPositive = 4,
    zNegative = 5
};


// x: 0, y: 1, z: 2
constexpr uint32_t axis(CubeNormal normal) {
    return (uint32_t)normal >> 1;
}

// x: 1, y: 0, z: 1
constexpr uint32_t widthAxis(uint32_t axis) {
    return (uint32_t)1 & ~axis;
}

// x: 2, y: 2, z: 0
constexpr uint32_t heightAxis(uint32_t axis) {
    return (uint32_t)2 & ~axis;
}

// +: 1, -: 0
constexpr uint32_t normalPositive(CubeNormal normal) {
    return (uint32_t)1 & ~(uint32_t)normal;
}

// +: 0, -: 1
constexpr uint32_t normalNegative(CubeNormal normal) {
    return (uint32_t)1 & (uint32_t)normal;
}

// +: 1, -: -1
constexpr int normalSign(CubeNormal normal) {
    return (int)1 | -(int)((uint32_t)normal & (uint32_t)1);
}



class Square {

public:
    Square(uint32_t x, uint32_t y, uint32_t z, uint32_t w, uint32_t h, CubeNormal normal, uint32_t colorID) :
        data1(x | (z << 13)),
        data2(y | ((w - 1) << 9) | ((h - 1) << 15) | ((uint32_t)normal << 21) | (colorID << 24)) {}

private:
    uint32_t data1; // x (13b), z (13b)
    uint32_t data2; // y (9b), width (6b), height (6b), normal (3b), color (8b)

};



// Mesh with all rectangles with the same normal and in the same chunk
class VoxelMesh {

public:
    glm::u32vec3 position;
    CubeNormal normal;
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

    glm::vec3 center() const {
        return (glm::vec3)position + glm::vec3(minX + maxX, minY + maxY, minZ + maxZ) / 2.0f;
    }

    glm::vec3 size() const {
        return glm::vec3(maxX - minX, maxY - minY, maxZ - minZ) / 2.0f;
    }

private:
    uint32_t minX;
    uint32_t minY;
    uint32_t minZ;
    uint32_t maxX;
    uint32_t maxY;
    uint32_t maxZ;
    
};



class MeshData {

public:
    MeshData(glm::vec3 center, glm::vec3 size, CubeNormal normal, uint32_t squareCount, uint32_t startSquare) :
        center(center),
        data1((uint32_t)normal | (squareCount << 3)),
        size(size),
        data2(startSquare) {}
    MeshData(const VoxelMesh& mesh, uint32_t startSquare) : MeshData(mesh.center(), mesh.size(), mesh.normal, mesh.squaresCount, startSquare) {};

public:
    glm::vec3 center;
    uint32_t data1; // normal (3b), squareCount (29b)
    glm::vec3 size;
    uint32_t data2; // startSquare (32b)

};



#endif