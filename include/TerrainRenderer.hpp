#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include <vector>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"
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
    GraphicsShader shader;
    std::vector<MeshData> meshData; // All meshes information (position, size, rectangles indices)
    std::vector<Square> squares; // All rectangles (position, width, height, normal)
    Buffer squaresBuffer;
    Buffer commandsBuffer;
    VertexArray vertexArray;
    Uniform graphicsPositionUniform;
    Uniform vpMatrixUniform;

    ComputeShader frustumCulling;
    Buffer meshDataBuffer;
    Buffer paramsBuffer;
    Uniform frustumPositionUniform;
    Uniform farPlaneUniform;
    Uniform leftPlaneUniform;
    Uniform rightPlaneUniform;
    Uniform upPlaneUniform;
    Uniform downPlaneUniform;
    GLuint workGroups;

};


#endif