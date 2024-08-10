#include "TerrainRenderer.hpp"

#include <vector>

#include "GLObjects/Render.hpp"
#include "GLObjects/Shader.hpp"
#include "Camera.hpp"
#include "VoxelMesh.hpp"

using namespace std;


TerrainRenderer::TerrainRenderer(Camera& camera)
  : camera(camera),
    shader(Shader("shaders/vertex.glsl", "shaders/fragment.glsl")),
    meshData(vector<MeshData>()),
    squares(vector<Square>()),
    squaresBuffer(StorageBuffer(0)),
    squaresIndicesBuffer(InstancesBuffer()),
    commandBuffer(CommandsBuffer(sizeof(IndirectDrawArgs))),
    vertexArray(VertexArray()),
    positionUniform(shader.getUniform("position")),
    vpMatrixUniform(shader.getUniform("vpMatrix")) {
        
    shader.getUniform("seed").setValue(0);
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
    threadGroups = meshData.size() / THREAD_GROUP_SIZE;

    // Create buffers
    squaresBuffer.setDataUnique(squares.data(), squares.size() * sizeof(Square), UniqueBufferUsage::none);

    uint32_t* _squaresIndices = new uint32_t[squares.size()];
    for (uint32_t i = 0; i < squares.size(); i++) _squaresIndices[i] = i;
    squaresIndicesBuffer.setDataUnique(_squaresIndices, squares.size() * sizeof(uint32_t), UniqueBufferUsage::none);
    delete[] _squaresIndices;
    vertexArray.setInstancesInt(0, squaresIndicesBuffer, IntAttributeType::uint32, 1, sizeof(uint32_t), 0);

    IndirectDrawArgs command = { 4, (uint)squares.size(), 0, 0 };
    commandBuffer.setDataUnique(&command, sizeof(command), UniqueBufferUsage::none);
}


void TerrainRenderer::render() {
    positionUniform.setValue(camera.position);
    vpMatrixUniform.setValue(camera.vpMatrix);
    renderIndirect(GeometryMode::triangleStrip, shader, vertexArray, commandBuffer, 0, 1, &squaresBuffer, 1);
}


void TerrainRenderer::dispose() {
    shader.dispose();
    squaresBuffer.dispose();
    squaresIndicesBuffer.dispose();
    commandBuffer.dispose();
    vertexArray.dispose();
}