#include "FPSCounter.hpp"

#include <iostream>

#include "TerminalRenderer.hpp"

using namespace std;


static constexpr float refreshRate = 0.5; // Time (in seconds) between each FPS update


FPSCounter::FPSCounter(TerminalRenderer& renderer) :
    TerminalRenderer::Component(renderer, 1),
    time(0),
    frames(0) {
}


void FPSCounter::update(float deltaTime) {
    time += deltaTime;
    frames++;
    if (time >= refreshRate) {
        string line = "FPS: " + to_string(frames / time);
        Component::update(&line);
        time = 0;
        frames = 0;
    }
}