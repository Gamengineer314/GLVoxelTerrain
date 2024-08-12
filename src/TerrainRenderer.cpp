#include "TerrainRenderer.hpp"

#include <cstdlib>
#include <vector>

#include "GLObjects/Render.hpp"
#include "GLObjects/Shader.hpp"
#include "Camera.hpp"
#include "VoxelMesh.hpp"

using namespace std;


TerrainRenderer::TerrainRenderer(Camera& camera)
  : camera(camera),
    shader(GraphicsShader("shaders/vertex.glsl", "shaders/fragment.glsl")),
    meshData(vector<MeshData>()),
    squares(vector<Square>()),
    squaresBuffer(InstancesBuffer()),
    commandsBuffer(IndirectDrawBuffer(sizeof(IndirectDrawArgs))),
    vertexArray(VertexArray()),
    graphicsPositionUniform(shader.getUniform("position")),
    vpMatrixUniform(shader.getUniform("vpMatrix")),
    frustrumCulling(ComputeShader("shaders/frustrumCulling.glsl")),
    meshDataBuffer(StorageBuffer(0)),
    paramsBuffer(ParametersBuffer(sizeof(uint32_t))),
    frustrumPositionUniform(frustrumCulling.getUniform("position")),
    farPlaneUniform(frustrumCulling.getUniform("farPlane")),
    leftPlaneUniform(frustrumCulling.getUniform("leftPlane")),
    rightPlaneUniform(frustrumCulling.getUniform("rightPlane")),
    upPlaneUniform(frustrumCulling.getUniform("upPlane")),
    downPlaneUniform(frustrumCulling.getUniform("downPlane")) {
        
    shader.getUniform("seed").setValue(rand() / (float)RAND_MAX);
    shader.getUniform("quadsInterleaving").setValue(QUADS_INTERLEAVING);
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
    squaresBuffer.setDataUnique(squares.data(), squares.size() * sizeof(Square), UniqueBufferUsage::none);
    vertexArray.setInstancesInt(0, squaresBuffer, IntAttributeType::uint32, 2, 2 * sizeof(uint32_t), 0);
    IndirectDrawArgs* commands = new IndirectDrawArgs[meshData.size()];
    fill(commands, commands + meshData.size(), IndirectDrawArgs { 4, 0, 0, 0 });
    commandsBuffer.setDataUnique(commands, meshData.size() * sizeof(IndirectDrawArgs), UniqueBufferUsage::none);
    delete[] commands;
    meshDataBuffer.setDataUnique(meshData.data(), meshData.size() * sizeof(MeshData), UniqueBufferUsage::none);
    paramsBuffer.setDataUnique(nullptr, sizeof(uint32_t), UniqueBufferUsage::none);
}


#include <iostream>
void TerrainRenderer::render() {
    // Frustrum culling
    frustrumPositionUniform.setValue(camera.position);
    farPlaneUniform.setValue(camera.farPlane);
    leftPlaneUniform.setValue(camera.leftPlane);
    rightPlaneUniform.setValue(camera.rightPlane);
    upPlaneUniform.setValue(camera.upPlane);
    downPlaneUniform.setValue(camera.downPlane);
    paramsBuffer.clearData(0, 4);
    ShaderBuffer frustrumCullingBuffers[] = { meshDataBuffer, StorageBuffer(1, commandsBuffer), CountersBuffer(0, paramsBuffer) };
    frustrumCulling.dispatch(workGroups, 1, 1, frustrumCullingBuffers, 3);
    memoryBarrier(MemoryBarrier::storage | MemoryBarrier::indirectCommand);

    // Draw
    graphicsPositionUniform.setValue(camera.position);
    vpMatrixUniform.setValue(camera.vpMatrix);
    renderIndirect(GeometryMode::triangleStrip, shader, vertexArray, commandsBuffer, 0, paramsBuffer, 0, meshData.size());
}


void TerrainRenderer::dispose() {
    shader.dispose();
    squaresBuffer.dispose();
    commandsBuffer.dispose();
    vertexArray.dispose();
    frustrumCulling.dispose();
    meshDataBuffer.dispose();
    paramsBuffer.dispose();
}