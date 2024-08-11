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
    squaresBuffer(StorageBuffer(0)),
    squaresIndicesBuffer(InstancesBuffer()),
    commandBuffer(IndirectDrawBuffer(sizeof(IndirectDrawArgs))),
    vertexArray(VertexArray()),
    graphicsPositionUniform(shader.getUniform("position")),
    vpMatrixUniform(shader.getUniform("vpMatrix")),
    frustrumCulling(ComputeShader("shaders/frustrumCulling.glsl")),
    meshDataBuffer(StorageBuffer(0)),
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
    squaresIndicesBuffer.setDataUnique(nullptr, squares.size() * sizeof(uint32_t), UniqueBufferUsage::none);
    vertexArray.setInstancesInt(0, squaresIndicesBuffer, IntAttributeType::uint32, 1, sizeof(uint32_t), 0);
    IndirectDrawArgs command = { 4, 0, 0, 0 };
    commandBuffer.setDataUnique(&command, sizeof(command), UniqueBufferUsage::none);
    meshDataBuffer.setDataUnique(meshData.data(), meshData.size() * sizeof(MeshData), UniqueBufferUsage::none);
}


void TerrainRenderer::render() {
    // Frustrum culling
    frustrumPositionUniform.setValue(camera.position);
    farPlaneUniform.setValue(camera.farPlane);
    leftPlaneUniform.setValue(camera.leftPlane);
    rightPlaneUniform.setValue(camera.rightPlane);
    upPlaneUniform.setValue(camera.upPlane);
    downPlaneUniform.setValue(camera.downPlane);
    commandBuffer.clearData(4, 4);
    ShaderBuffer frustrumCullingBuffers[] = { meshDataBuffer, CountersBuffer(0, commandBuffer), StorageBuffer(1, squaresIndicesBuffer) };
    frustrumCulling.dispatch(workGroups, 1, 1, frustrumCullingBuffers, 3);
    memoryBarrier(MemoryBarrier::storage | MemoryBarrier::indirectCommand);

    // Draw
    graphicsPositionUniform.setValue(camera.position);
    vpMatrixUniform.setValue(camera.vpMatrix);
    renderIndirect(GeometryMode::triangleStrip, shader, vertexArray, commandBuffer, 0, 1, &squaresBuffer, 1);
}


void TerrainRenderer::dispose() {
    shader.dispose();
    squaresBuffer.dispose();
    squaresIndicesBuffer.dispose();
    commandBuffer.dispose();
    vertexArray.dispose();
    frustrumCulling.dispose();
    meshDataBuffer.dispose();
}