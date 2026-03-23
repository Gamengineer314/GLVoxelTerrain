#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "TerminalRenderer.hpp"


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


#endif