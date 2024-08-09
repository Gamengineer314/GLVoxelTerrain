#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "TerminalRenderer.hpp"

#define REFRESH_RATE 0.5f // Time (in seconds) between each FPS update


class FPSCounter : private TerminalRenderer::Component {

public:
    /**
     * @brief Create a new FPS counter
     * @param renderer Terminal renderer to use
    **/
    FPSCounter(TerminalRenderer& renderer);

    /**
     * @brief FPSCounter update (must be called each frame)
     * @param deltaTime Last frame duration (in seconds)
    **/
    void update(float deltaTime);

private:
    float time;
    int frames;

};


#endif // FPS_COUNTER_H