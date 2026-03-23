#include "TerrainRenderer.hpp"

#include <cstdlib>
#include <vector>

#include "GLObjects/OpenGL.hpp"
#include "Camera.hpp"
#include "VoxelMesh.hpp"

using namespace gl;
using namespace glm;
using namespace std;


static constexpr int threadGroupSize = 64; // Number of threads in a work group for the compute shader
static constexpr float quadsInterleaving = 0.05f; // Remove small (1 pixel) gaps between triangles


TerrainRenderer::TerrainRenderer(Camera& camera) :
    camera(camera),
    shader("shaders/vertex.glsl", "shaders/fragment.glsl"),
    graphicsPositionUniform(shader, "position"),
    vpMatrixUniform(shader, "vpMatrix"),
    frustumCulling("shaders/frustumCulling.glsl"),
    frustumPositionUniform(frustumCulling, "position"),
    farPlaneUniform(frustumCulling, "farPlane"),
    leftPlaneUniform(frustumCulling, "leftPlane"),
    rightPlaneUniform(frustumCulling, "rightPlane"),
    upPlaneUniform(frustumCulling, "upPlane"),
    downPlaneUniform(frustumCulling, "downPlane") {
    Uniform(shader, "seed").setValue(shader, rand() / (float)RAND_MAX);
    Uniform(shader, "quadsInterleaving").setValue(shader, quadsInterleaving);
    vertexArray.use();
    commandsBuffer.use(BufferType::indirectDraw);
    paramsBuffer.use(BufferType::parameters);
    meshDataBuffer.use(ShaderBufferType::storage, 0);
    commandsBuffer.use(ShaderBufferType::storage, 1);
    paramsBuffer.use(ShaderBufferType::counters, 0);
}


void TerrainRenderer::addMeshes(const vector<VoxelMesh>& meshes, const vector<Square>& squares) {
    uint32_t startSquare = this->squares.size();
    for (VoxelMesh mesh : meshes) {
        meshData.push_back(MeshData(mesh, startSquare));
        startSquare += mesh.squaresCount;
    }
    this->squares.insert(this->squares.end(), squares.begin(), squares.end());
}


void TerrainRenderer::prepareRender() {
    // Add empty meshes at the end to have a size multiple of THREAD_GROUP_SIZE
    while (meshData.size() % threadGroupSize != 0) meshData.push_back(MeshData(vec3(0), vec3(0), CubeNormal::xPositive, 0, 0));
    workGroups = meshData.size() / threadGroupSize;

    // Create buffers
    squaresBuffer.setDataUnique(squares.data(), squares.size(), UniqueBufferUsage::none);
    IndirectDrawArgs* commands = new IndirectDrawArgs[meshData.size()];
    fill(commands, commands + meshData.size(), IndirectDrawArgs { 4, 0, 0, 0 });
    commandsBuffer.setDataUnique(commands, meshData.size(), UniqueBufferUsage::none);
    delete[] commands;
    meshDataBuffer.setDataUnique(meshData.data(), meshData.size(), UniqueBufferUsage::none);
    paramsBuffer.setDataUnique<uint32_t>(nullptr, 1, UniqueBufferUsage::none);

    // Create vertex array
    vertexArray.setBuffer(0, squaresBuffer, 2 * sizeof(uint32_t), 0, 1);
    vertexArray.setAttributeFormat(0, IntAttributeType::uint32, 2, 0);
    vertexArray.setAttributeBuffer(0, 0);
}


void TerrainRenderer::render() {
    // Frustum culling
    frustumPositionUniform.setValue(frustumCulling, camera.position);
    farPlaneUniform.setValue(frustumCulling, camera.farPlane);
    leftPlaneUniform.setValue(frustumCulling, camera.leftPlane);
    rightPlaneUniform.setValue(frustumCulling, camera.rightPlane);
    upPlaneUniform.setValue(frustumCulling, camera.upPlane);
    downPlaneUniform.setValue(frustumCulling, camera.downPlane);
    paramsBuffer.clearData(sizeof(uint32_t));
    frustumCulling.use();
    compute(workGroups);
    barrier(MemoryBarrier::indirectCommand);

    // Draw
    graphicsPositionUniform.setValue(shader, camera.position);
    vpMatrixUniform.setValue(shader, camera.vpMatrix);
    shader.use();
    drawIndirectParam(GeometryMode::triangleStrip, meshData.size());
}