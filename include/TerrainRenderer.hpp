#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H

#include <vector>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"
#include "Camera.hpp"
#include "VoxelMesh.hpp"

using namespace std;

// Number of threads in a work group for the compute shader
#define THREAD_GROUP_SIZE 64
// Remove small (1 pixel) gaps between triangles
#define QUADS_INTERLEAVING 0.05f


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
    void addMeshes(vector<VoxelMesh>& meshes, vector<Square>& squares);

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

    /**
     * @brief Delete the terrain renderer
    **/
    void dispose();

private:
    Camera& camera;
    GraphicsShader shader;
    vector<MeshData> meshData; // All meshes information (position, size, rectangles indices)
    vector<Square> squares; // All rectangles (position, width, height, normal)
    InstancesBuffer squaresBuffer;
    IndirectDrawBuffer commandsBuffer;
    VertexArray vertexArray;
    Shader::Uniform graphicsPositionUniform;
    Shader::Uniform vpMatrixUniform;

    ComputeShader frustrumCulling;
    StorageBuffer meshDataBuffer;
    ParametersBuffer paramsBuffer;
    Shader::Uniform frustrumPositionUniform;
    Shader::Uniform farPlaneUniform;
    Shader::Uniform leftPlaneUniform;
    Shader::Uniform rightPlaneUniform;
    Shader::Uniform upPlaneUniform;
    Shader::Uniform downPlaneUniform;
    GLuint workGroups;

};


#endif // TERRAIN_RENDERER_H