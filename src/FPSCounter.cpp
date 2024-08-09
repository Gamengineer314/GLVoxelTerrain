#include "FPSCounter.hpp"

#include <iostream>

#include "TerminalRenderer.hpp"

using namespace std;


FPSCounter::FPSCounter(TerminalRenderer& renderer) :
    TerminalRenderer::Component(renderer, 1),
    time(0),
    frames(0) {
}


void FPSCounter::update(float deltaTime) {
    time += deltaTime;
    frames++;
    if (time >= REFRESH_RATE) {
        string line = "FPS: " + to_string(frames / time);
        Component::update(&line);
        time = 0;
        frames = 0;
    }
}