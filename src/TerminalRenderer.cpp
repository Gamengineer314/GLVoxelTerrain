#include "TerminalRenderer.hpp"

#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/ioctl.h>
#endif

using namespace std;



int getTerminalWidth() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO sbInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sbInfo);
    return sbInfo.dwSize.X;
#elif defined(__linux__) || defined(__APPLE__)
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    return (int)(w.ws_col);
#endif
}



TerminalRenderer::TerminalRenderer(FILE* output) :
    output(output),
    lines(vector<char>()),
    lineCount(0),
    width(getTerminalWidth()) {   
}


void TerminalRenderer::render() {
    fprintf(output, "\u001b[%dA", lineCount);
    fwrite(lines.data(), 1, lines.size(), output);
}


int TerminalRenderer::addComponent(int lineSize) {
    int lineStart = lineCount;
    lineCount += lineSize;
    lines.insert(lines.end(), lineSize * (width - 1), ' ');
    lines.push_back('\n');
    fwrite(lines.data() + lineStart * width, 1, lineSize * width, output);
    return lineStart;
}


void TerminalRenderer::updateComponent(const string* componentLines, int lineStart, int lineSize) {
    for (int i = 0; i < lineSize; i++) {
        size_t length = min(componentLines[i].length(), (size_t)width - 1);
        memcpy(lines.data() + (lineStart + i) * width, componentLines[i].data(), length);
        memset(lines.data() + (lineStart + i) * width + length, ' ', width - 1 - length);
    }
}



TerminalRenderer::Component::Component(TerminalRenderer& renderer, int lineSize) :
    renderer(renderer),
    lineStart(renderer.addComponent(lineSize)),
    lineSize(lineSize) {
}


void TerminalRenderer::Component::update(const string* lines) {
    renderer.updateComponent(lines, lineStart, lineSize);
}