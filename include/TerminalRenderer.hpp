#ifndef TERMINAL_RENDERER_H
#define TERMINAL_RENDERER_H

#include <cstdio>
#include <iostream>
#include <vector>

using std::vector, std::string;


class TerminalRenderer {

public:
    class Component {

    public:
        /**
         * @brief Create a new component
         * @param renderer Terminal renderer to use
         * @param lineSize Number of lines for the component
        **/
        Component(TerminalRenderer& renderer, int lineSize);

        /**
         * @brief Update the component
         * @param lines New component text
        **/
        void update(const std::string* lines);

    private:
        TerminalRenderer& renderer;
        int lineStart;
        int lineSize;

    };

    /**
     * @brief Create a new terminal renderer
    **/
    TerminalRenderer(FILE* output);

    /**
     * @brief Render updated components in the terminal
    **/
    void render();

private:
    FILE* output;
    std::vector<char> lines;
    int lineCount;
    int width;

    /**
     * @brief Add a component to the terminal
     * @param lineSize Number of lines for the component
     * @return Line start of the component
    **/
    int addComponent(int lineSize);

    /**
     * @brief Update a component
     * @param componentLines New component text
     * @param lineStart Line start of the component
     * @param lineSize Number of lines of the component
    **/
    void updateComponent(const std::string* componentLines, int lineStart, int lineSize);

};


#endif