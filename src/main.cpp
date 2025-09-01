#include <cstdint>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "GLObjects/Window.hpp"
#include "GLObjects/Command.hpp"
#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "Camera.hpp"
#include "CameraController.hpp"
#include "VoxelMesh.hpp"
#include "TerrainRenderer.hpp"
#include "GenerateTerrain.hpp"
#include "GenerateMesh.hpp"
#include "Constants.hpp"
#include "TerminalRenderer.hpp"
#include "FPSCounter.hpp"

using namespace std;
using namespace this_thread;
using namespace chrono;
using namespace chrono_literals;
using namespace glm;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define TITLE "Voxel Terrain"
#define BACKGROUND_COLOR 0, 0.8, 1.0


int main() {
    // Generate terrain
    vector<int> IDs;
    uint32_t* IDIndexes = new uint32_t[HORIZONTAL_SIZE * HORIZONTAL_SIZE + 1];
    generateTerrain(IDs, IDIndexes);
    vector<VoxelMesh> meshes;
    vector<Square> squares;
    generateMesh(0, 0, HORIZONTAL_CHUNKS, HORIZONTAL_CHUNKS, IDs.data(), IDIndexes, meshes, squares);
    delete[] IDIndexes;
    
    // Initialize objects
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
    commandInit(WINDOW_WIDTH, WINDOW_HEIGHT);
    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, 60, 0.1, 1500, vec3(2048, 100, 2048), 0, 0);
    CameraController controller(window, camera, WINDOW_WIDTH, WINDOW_HEIGHT);
    TerrainRenderer renderer(camera);
    renderer.addMeshes(meshes, squares);
    renderer.prepareRender();
    TerminalRenderer terminal(stdout);
    FPSCounter fpsCounter(terminal);
    
    // Main loop
    system_clock::time_point lastTime = system_clock::now();
    while (!window.closed()) {
        // Delta time
        system_clock::time_point time = system_clock::now();
        float deltaTime = duration_cast<microseconds>(time - lastTime).count() / 1000000.0f;
        lastTime = time;

        // Update
        controller.update(deltaTime);
        commandBackground(BACKGROUND_COLOR);
        renderer.render();
        fpsCounter.update(deltaTime);
        terminal.render();
        window.update();
    }
}
