#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <tuple>
#include <chrono>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GRIDSIZE 25

using namespace std;
using namespace std::chrono;

class Canvas
{
public:
    Canvas(GamePlay *game, int width, int height);
    void init();
    void recalculateBuffers();
    void calcInput();

    GamePlay *game;
    int **board;

private:
    GLFWwindow *window;

    int width, height;

    GLfloat *foodSnakePosition;
    GLfloat *foodSnakeColor;

    high_resolution_clock::time_point pastTime;

    high_resolution_clock::time_point currentTime;

    duration<double, milli> dur;

    int fpsCounter;

    tuple<int, int> direction;
};

#endif // CANVAS_H
