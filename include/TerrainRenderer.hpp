#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include <vector>

#include "GLObjects/OpenGL.hpp"
#include "Camera.hpp"
#include "VoxelMesh.hpp"


class TerrainRenderer {
public:
    /**
     * @brief Create a new voxel terrain renderer
     * @param camera Camera to use to render the terrain
    **/
    explicit TerrainRenderer(Camera& camera);

    /**
     * @brief Add meshes to render (before starting to render)
     * @param meshes Meshes to add
     * @param squares Squares in the added meshes
    **/
    void addMeshes(const std::vector<VoxelMesh>& meshes, const std::vector<Square>& squares);

    /**
     * @brief 
     * Prepare for render.
     * Must be called before any call to render() and after every call to addMeshes()
    **/
    void prepareRender();

    /**
     * @brief Render the terrain
    **/
    void render();

private:
    Camera& camera;
    gl::GraphicsShader shader;
    std::vector<MeshData> meshData; // All meshes information (position, size, rectangles indices)
    std::vector<Square> squares; // All rectangles (position, width, height, normal)
    gl::Buffer squaresBuffer;
    gl::Buffer commandsBuffer;
    gl::VertexArray vertexArray;
    gl::Uniform graphicsPositionUniform;
    gl::Uniform vpMatrixUniform;

    gl::ComputeShader frustumCulling;
    gl::Buffer meshDataBuffer;
    gl::Buffer paramsBuffer;
    gl::Uniform frustumPositionUniform;
    gl::Uniform farPlaneUniform;
    gl::Uniform leftPlaneUniform;
    gl::Uniform rightPlaneUniform;
    gl::Uniform upPlaneUniform;
    gl::Uniform downPlaneUniform;
    uint32_t workGroups;
};


#endif