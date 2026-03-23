#include <cstdint>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "GLObjects/Window.hpp"
#include "GLObjects/OpenGL.hpp"
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


static constexpr int windowWidth = 1920;
static constexpr int windowHeight = 1080;
static constexpr const char* title = "Voxel Terrain";
static constexpr float backgroundRed = 0, backgroundGreen = 0.8, backgroundBlue = 1.0;


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
    Window window(windowWidth, windowHeight, title);
    gl::init(windowWidth, windowHeight);
    Camera camera(windowWidth, windowHeight, 60, 0.1, 9999, vec3(0, 400, 0), pi<float>() / 6, pi<float>() / 4);
    CameraController controller(window, camera, windowWidth, windowHeight);
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
        gl::setBackground(backgroundRed, backgroundGreen, backgroundBlue);
        renderer.render();
        fpsCounter.update(deltaTime);
        terminal.render();
        window.update();
    }
}
