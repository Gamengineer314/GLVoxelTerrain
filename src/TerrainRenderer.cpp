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
    squaresBuffers { StorageBuffer(0), StorageBuffer(1) },
    indicesBuffer(IndicesBuffer()),
    commandBuffer(CommandsBuffer(sizeof(IndirectIndexedDrawArgs))),
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
    squaresBuffers[0].setDataUnique(squares.data(), squares.size() * sizeof(Square), UniqueBufferUsage::none);

    uint32_t* _squaresIndices = new uint32_t[squares.size()];
    for (uint32_t i = 0; i < squares.size(); i++) _squaresIndices[i] = i;
    squaresBuffers[1].setDataUnique(_squaresIndices, squares.size() * sizeof(uint32_t), UniqueBufferUsage::none);
    delete[] _squaresIndices;

    uint8_t indices[] = { 0, 1, 2, 2, 1, 3 };
    indicesBuffer.setDataUnique(indices, sizeof(indices), UniqueBufferUsage::none);

    IndirectIndexedDrawArgs command = { 6, (uint)squares.size(), 0, 0, 0 };
    commandBuffer.setDataUnique(&command, sizeof(command), UniqueBufferUsage::none);

    vertexArray.setIndices(indicesBuffer, IndexType::uint8);
}


void TerrainRenderer::render() {
    positionUniform.setValue(camera.position);
    vpMatrixUniform.setValue(camera.vpMatrix);
    renderIndirect(GeometryMode::triangles, shader, vertexArray, commandBuffer, 0, 1, squaresBuffers, 2);
}


void TerrainRenderer::dispose() {
    shader.dispose();
    squaresBuffers[0].dispose();
    squaresBuffers[1].dispose();
    indicesBuffer.dispose();
    commandBuffer.dispose();
    vertexArray.dispose();
}