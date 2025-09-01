#include "TerrainRenderer.hpp"

#include <cstdlib>
#include <vector>

#include "GLObjects/Command.hpp"
#include "GLObjects/Shader.hpp"
#include "Camera.hpp"
#include "VoxelMesh.hpp"

using namespace std;


TerrainRenderer::TerrainRenderer(Camera& camera)
  : camera(camera),
    shader("shaders/vertex.glsl", "shaders/fragment.glsl"),
    graphicsPositionUniform(shader, "position"),
    vpMatrixUniform(shader, "vpMatrix"),
    frustrumCulling("shaders/frustrumCulling.glsl"),
    frustrumPositionUniform(frustrumCulling, "position"),
    farPlaneUniform(frustrumCulling, "farPlane"),
    leftPlaneUniform(frustrumCulling, "leftPlane"),
    rightPlaneUniform(frustrumCulling, "rightPlane"),
    upPlaneUniform(frustrumCulling, "upPlane"),
    downPlaneUniform(frustrumCulling, "downPlane") {
    Uniform(shader, "seed").setValue(rand() / (float)RAND_MAX);
    Uniform(shader, "quadsInterleaving").setValue(QUADS_INTERLEAVING);
    frustrumCulling.setBuffer(0, ShaderBufferType::storage, meshDataBuffer);
    frustrumCulling.setBuffer(1, ShaderBufferType::storage, commandsBuffer);
    frustrumCulling.setBuffer(0, ShaderBufferType::counters, paramsBuffer);
}


void TerrainRenderer::addMeshes(vector<VoxelMesh>& meshes, vector<Square>& squares) {
    uint32_t startSquare = this->squares.size();
    for (VoxelMesh mesh : meshes) {
        meshData.push_back(MeshData(mesh, startSquare));
        startSquare += mesh.squaresCount;
    }
    this->squares.insert(this->squares.end(), squares.begin(), squares.end());
}


void TerrainRenderer::prepareRender() {
    // Add empty meshes at the end to have a size multiple of THREAD_GROUP_SIZE
    while (meshData.size() % THREAD_GROUP_SIZE != 0) meshData.push_back(MeshData(vec3(0), vec3(0), 0, 0, 0));
    workGroups = meshData.size() / THREAD_GROUP_SIZE;

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
    // Frustrum culling
    frustrumPositionUniform.setValue(camera.position);
    farPlaneUniform.setValue(camera.farPlane);
    leftPlaneUniform.setValue(camera.leftPlane);
    rightPlaneUniform.setValue(camera.rightPlane);
    upPlaneUniform.setValue(camera.upPlane);
    downPlaneUniform.setValue(camera.downPlane);
    paramsBuffer.clearData(sizeof(uint32_t));
    commandCompute(frustrumCulling, workGroups);
    commandBarrier(MemoryBarrier::indirectCommand);

    // Draw
    graphicsPositionUniform.setValue(camera.position);
    vpMatrixUniform.setValue(camera.vpMatrix);
    commandDrawIndirect(GeometryMode::triangleStrip, shader, vertexArray, commandsBuffer, paramsBuffer, meshData.size());
}